#include <mitsuba/core/platform.h>
// Mitsuba's "Assert" macro conflicts with Xerces' XSerializeEngine::Assert(...).
// This becomes a problem when using a PCH which contains mitsuba/core/logger.h
#if defined(Assert)
# undef Assert
#endif
#include <xercesc/parsers/SAXParser.hpp>
#include <mitsuba/core/sched_remote.h>
#include <mitsuba/core/sstream.h>
#include <mitsuba/core/fresolver.h>
#include <mitsuba/core/fstream.h>
#include <mitsuba/core/appender.h>
#include <mitsuba/core/sshstream.h>
#include <mitsuba/core/shvector.h>
#include <mitsuba/core/statistics.h>
#include <mitsuba/render/renderjob.h>
#include <mitsuba/render/scenehandler.h>
#include <fstream>
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#if defined(__WINDOWS__)
#include <mitsuba/core/getopt.h>
#include <winsock2.h>
#else
#include <signal.h>
#endif
#include <mitsuba/render/scene.h>
#include <mitsuba/render/integrator.h>
#include <mitsuba/render/shape.h>
#include <mitsuba/render/bsdf.h>
#include <mitsuba/render/staticsampler.h>
#include <mitsuba/core/plugin.h>
#include <mitsuba/core/bitmap.h>
#include <mitsuba/core/fstream.h>
#include <mitsuba/render/util.h>
#include <emca/datatypes.h>
#include <emca/dataapimitsuba.h>
#include <emca/mesh.h>
#include <emca/camera.h>
#include <emca/rendercontroller.h>
#include <emca/server.h>
#include <emca/renderinfo.h>
#include <emca/toolsphericalview.h>
MTS_NAMESPACE_BEGIN
using XERCES_CPP_NAMESPACE::SAXParser;
ref<RenderQueue> renderQueue = NULL;
emca::Server *emcaServer = nullptr;
#if !defined(__WINDOWS__)
/* Handle the hang-up signal and write a partially rendered image to disk */
void signalHandler(int signal) {
    if (signal == SIGHUP && renderQueue.get()) {
        renderQueue->flush();
    } else if (signal == SIGFPE) {
        SLog(EWarn, "Caught a floating-point exception!");
        #if defined(MTS_DEBUG_FP)
        /* Generate a core dump! */
        abort();
        #endif
    } else if (signal == SIGINT && emcaServer) {
    	emcaServer->shutdown();
    	exit(EXIT_SUCCESS);
    }
}
#endif
class FlushThread : public Thread {
public:
    FlushThread(int timeout) : Thread("flush"),
        m_flag(new WaitFlag()),
        m_timeout(timeout) { }
    void run() {
        while (!m_flag->get()) {
            m_flag->wait(m_timeout * 1000);
            renderQueue->flush();
        }
    }
    void quit() {
        m_flag->set(true);
        join();
    }
private:
    ref<WaitFlag> m_flag;
    int m_timeout;
};
class CopyMitsuba : public emca::RenderController {
public:
	CopyMitsuba(int argc, char **argv)
		: emca::RenderController() {
		int retval = mitsuba_init(argc, argv);
		m_rendered = false;
		m_argc = argc;
		SLog(EInfo, "Retval from init: %d", retval);
		if(retval < 0) {
			SLog(EInfo, "Init error.");
			exit(retval);
		}
		SLog(EInfo, "Starting Server for Explorer of Monte-Carlo based Algorithms ...");
	}

	virtual ~CopyMitsuba() { }

	void runPreprocess() {
		if (!m_rendered) {
			if(!m_scene->preprocess(renderQueue, m_thr, m_thr->getSceneResID(), m_thr->getSensorResID(), m_thr->getSamplerResID())) {
				std::cout << "PREPROCESS FAILED!" << std::endl;
			}
			m_rendered = true;
		}
	}

	void renderImage() {
		mitsuba_render();
		m_rendered = true;
	}

	void renderPixel(unsigned int x, unsigned int y, int sampleCount) {
		// if images was not rendered before initialize scene and start pre-processing
		runPreprocess();
		// basic render process copied from render/integrator.cpp
		Point2i pixel(x, y);
		ref<Sampler> sampler = StaticDeterministicPixelSampler::getSamplerToPixel(x, y, sampleCount);
		// copied path tracer
		MonteCarloIntegrator *integrator = static_cast<MonteCarloIntegrator *>(m_scene->getIntegrator());
		// configure sampler
		integrator->configureSampler(m_scene.get(), sampler.get());
		const Sensor *sensor = m_scene->getSensor();
		Float diffScaleFactor = 1.0f /
				std::sqrt((Float) sampleCount);
		bool needsApertureSample = sensor->needsApertureSample();
		bool needsTimeSample = sensor->needsTimeSample();
		Point2 apertureSample(0.5f);
		Float timeSample = 0.5f;
		RayDifferential sensorRay;
		uint32_t queryType = RadianceQueryRecord::ESensorRay;
		/* Don't compute an alpha channel if we don't have to */
		if (!sensor->getFilm()->hasAlpha())
			queryType &= ~RadianceQueryRecord::EOpacity;
		RadianceQueryRecord rRec(m_scene, sampler.get());
		sampler->generate(pixel);
		Spectrum spec;
		Spectrum L;
		Spectrum mcL = Spectrum(0.0);
		// work with provided data API interface for mitsuba
		emca::DataApiMitsuba *dataApiMitsuba = emca::DataApiMitsuba::getInstance();
		for (int sampleIdx = 0; sampleIdx < sampleCount; sampleIdx++) {
			dataApiMitsuba->setSampleIdx(sampleIdx);
			rRec.newQuery(queryType, sensor->getMedium());
			Point2 samplePos(Point2(pixel) + Vector2(rRec.nextSample2D()));
			if (needsApertureSample)
				apertureSample = rRec.nextSample2D();
			if (needsTimeSample)
				timeSample = rRec.nextSample1D();
			spec = sensor->sampleRayDifferential(
					sensorRay, samplePos, apertureSample, timeSample);
			sensorRay.scaleDifferential(diffScaleFactor);
			// take sensor ray origin as start ray instead of camera origin
			dataApiMitsuba->setPathOrigin(Point3f(sensorRay.o.x, sensorRay.o.y, sensorRay.o.z));
			L = integrator->Li(sensorRay, rRec);
			dataApiMitsuba->addPathInfoSpectrum("Segment_" + std::to_string(sampleIdx), L);
			spec *= L;
			mcL += spec;
			sampler->advance();
		}
		// final estimated value
		mcL /= (Float)sampleCount;
	}

	void sendRenderInformation(emca::Stream *stream) {
		fs::path pathToFile = m_scene->getSourceFile();
		fs::path fileName = pathToFile.filename();
		fs::path pathToOutputFile = m_scene->getDestinationFile();
		fs::path extension = pathToOutputFile.extension();
		Sampler *sampler = m_scene->getSampler();
		int sampleCount = sampler->getSampleCount();

		emca::RenderInfo(fileName.string(), pathToOutputFile.string(),
				extension.string(), sampleCount).serialize(stream);
	}

	void sendCameraData(emca::Stream *stream) {
		// if images was not rendered before, initialize scene and start pre-processing
		runPreprocess();

		// get camera settings from rendering system
		Sensor *sensor = m_scene->getSensor();
		Properties props = sensor->getProperties();
		float nearClip = props.getFloat("nearClip", 0.1);
		float farClip = props.getFloat("farClip", 100);
		float focusDist = props.getFloat("focusDistance", 10);
		float fov = props.getFloat("fov", 40);
		Transform t = props.getTransform("toWorld", Transform());
		Matrix4x4 mat = t.getMatrix();
		emca::Point3f origin = emca::Point3f(mat(0, 3), mat(1, 3), mat(2, 3));
		emca::Vec3f up = emca::Vec3f(mat(0, 1), mat(1, 1), mat(2, 1));
		emca::Vec3f dir = emca::Vec3f(
				origin.x() + mat(0, 2),
				origin.y() + mat(1, 2),
				origin.z() + mat(2, 2));

		// Initialize and serialize EMCA camera
		emca::Camera(nearClip, farClip, focusDist, fov,
				up, dir, origin).serialize(stream);
	}
	void sendMeshData(emca::Stream* stream) {
		// if images was not rendered before initialize scene and start pre-processing
		runPreprocess();
		ref_vector<Shape> shapes = m_scene->getShapes();
		for(unsigned int i = 0; i < shapes.size(); ++i) {
			emca::Mesh emcaMesh = emca::Mesh();
			// get vertex positions
			ref<TriMesh> triMesh = shapes[i]->createTriMesh();
            //SLog(EInfo, "transferring shape %s with %zu vertices...", shapes[i]->getName().c_str(), triMesh->getVertexCount());
			Point *vertexList = triMesh->getVertexPositions();
#ifndef SINGLE_PRECISION
			for(uint32_t vert = 0; vert < triMesh->getVertexCount(); vert++) {
				emca::Point3f emcaPoint(
						vertexList[vert].x,
						vertexList[vert].y,
						vertexList[vert].z);
				emcaMesh.addVertexPosition(emcaPoint);
			}
#else
            //mitsuba uses a compatible point type, no need to convert data one-by-one
            emcaMesh.addVertices(reinterpret_cast<emca::Point3f*>(vertexList), triMesh->getVertexCount());
#endif
			// get triangles indices
			Triangle *triangles = triMesh->getTriangles();
            //mitsuba uses a compatible point type, no need to convert data one-by-one
            emcaMesh.addTriangles(reinterpret_cast<emca::Vec3i*>(triangles), triMesh->getTriangleCount());
			// define color of object
			Shape *s = shapes[i];
			Sampler *sampler = m_scene->getSampler();
			emca::Color4f diffRef;
			emca::Color4f specRef;
			if(s->isEmitter()) {
				diffRef = emca::Color4f(1, 1, 1);
				specRef = emca::Color4f(0, 0, 0);
			} else if(s->isSensor()){
				diffRef = emca::Color4f(1, 1, 1);
				specRef = emca::Color4f(0, 0, 0);
			} else if(s->isMediumTransition()) {
				diffRef = emca::Color4f(1, 1, 1);
				specRef = emca::Color4f(1, 1, 1);
			} else {
				if(s->hasBSDF()) {
					PositionSamplingRecord pRec;
					s->samplePosition(pRec, sampler->next2D());
					Intersection its;
					its.p = pRec.p;
					its.wi = warp::squareToCosineHemisphere(sampler->next2D());
					const BSDF* bsdf = s->getBSDF();
					Spectrum diffuse = bsdf->getDiffuseReflectance(its);
					Spectrum specular = bsdf->getSpecularReflectance(its);
					diffRef = emca::Color4f(diffuse[0], diffuse[1], diffuse[2]);
					specRef = emca::Color4f(specular[0], specular[1], specular[2]);
				} else {
					float diffuseGamble = sampler->next1D();
					float specularGamble = sampler->next1D();
					diffRef = emca::Color4f(diffuseGamble, diffuseGamble, diffuseGamble);
					specRef = emca::Color4f(specularGamble, specularGamble, specularGamble);
				}
			}
			// add color to vsdmesh
			emcaMesh.setDiffuseColor(diffRef);
			emcaMesh.setSpecularColor(specRef);
			// send mesh to client
			emcaMesh.serialize(stream);
		}
	}

	void updateSampleCount(int sampleCount) {
		Properties samplerProps{"deterministic"};
		samplerProps.setInteger("sampleCount", sampleCount);
		ref<PluginManager> pluginManager = PluginManager::getInstance();
		ref<Sampler> sampler = static_cast<Sampler*>(pluginManager->createObject(MTS_CLASS(Sampler), samplerProps));
		m_scene->setSampler(sampler.get());
	}

	void help() {
		cout << "Help msg!" << endl;
	}

	void mitsuba_render() {
		int i = 1;
		int flushTimer = -1;
		int numParallelScenes = 1;
		// FlushThread is always -1 in this case ?!
		ref<FlushThread> flushThread;
		if (flushTimer > 0) {
			flushThread = new FlushThread(flushTimer);
			flushThread->start();
		}
		// start rendering (main mitsuba process)
		renderQueue = new RenderQueue();
		m_thr = new RenderJob(formatString("ren%i", 0), m_scene, renderQueue);
		m_thr->start();
		Statistics::getInstance()->printStats();
		renderQueue->waitLeft(numParallelScenes-1);
		if (i+1 < m_argc && numParallelScenes == 1)
			Statistics::getInstance()->resetAll();
		/* Wait for all render processes to finish */
		renderQueue->waitLeft(0);
		if (flushThread)
			flushThread->quit();
		renderQueue = NULL;
	}
	Scene* getScene() { return m_scene; }
	fs::path getDestinationFile() { return m_scene->getDestinationFile(); }
private:
	int mitsuba_init(int argc, char **argv) {
		 try {
		        /* Default settings */
		        std::string nodeName = getHostName(),
		                    networkHosts = "", destFile="";
		        bool progressBars = true;
		        ref<FileResolver> fileResolver = Thread::getThread()->getFileResolver();
		        std::map<std::string, std::string, SimpleStringOrdering> parameters;
		        int blockSize = 32;
		        ProgressReporter::setEnabled(progressBars);
		#if !defined(__WINDOWS__)
		            /* Initialize signal handlers */
		            struct sigaction sa;
		            sa.sa_handler = signalHandler;
		            sigemptyset(&sa.sa_mask);
		            sa.sa_flags = 0;
		            if (sigaction(SIGHUP, &sa, NULL))
		                SLog(EError, "Could not install a custom signal handler!");
		            if (sigaction(SIGFPE, &sa, NULL))
		                SLog(EError, "Could not install a custom signal handler!");
		#endif
		        /* Prepare for parsing scene descriptions */
		        SAXParser* parser = new SAXParser();
		        fs::path schemaPath = fileResolver->resolveAbsolute("data/schema/scene.xsd");
		        /* Check against the 'scene.xsd' XML Schema */
		        parser->setDoSchema(true);
		        parser->setValidationSchemaFullChecking(true);
		        parser->setValidationScheme(SAXParser::Val_Always);
		        parser->setExternalNoNamespaceSchemaLocation(schemaPath.c_str());
		        /* Set the handler */
		        SceneHandler *handler = new SceneHandler(parameters);
		        parser->setDoNamespaces(true);
		        parser->setDocumentHandler(handler);
		        parser->setErrorHandler(handler);
		        int i = 1;
				fs::path
					filename = fileResolver->resolve(argv[i]),
					filePath = fs::absolute(filename).parent_path(),
					baseName = filename.stem();
				ref<FileResolver> frClone = fileResolver->clone();
				frClone->prependPath(filePath);
				Thread::getThread()->setFileResolver(frClone);
				SLog(EInfo, "Parsing scene description from \"%s\" ..", argv[i]);
				parser->parse(filename.c_str());
				m_scene = handler->getScene();
				m_scene->setSourceFile(filename);
				m_scene->setDestinationFile(destFile.length() > 0 ?
					fs::path(destFile) : (filePath / baseName));
				m_scene->setBlockSize(blockSize);
				// init scene and kd-tree
				m_scene->initialize();
				renderQueue = new RenderQueue();
				m_thr = new RenderJob(formatString("ren%i", 0), m_scene, renderQueue);
				// DEBUG add scene here to tool sphericalView
				//*******
				//emca::SphericalView *swTool = static_cast<emca::SphericalView*>(this->getDataApi()->getToolById(66));
				//swTool->setScene(m_scene);
				//*******
				// check if sampler is set to seed sampler,
				// necessary to reproduce render images
				ref<Sampler> sampler = m_scene->getSampler();
                if(false && sampler->getClass()->getName() != "DeterministicSampler") {
					std::cerr << "Set Deterministic Sampler as default Sampler in your scene!" << endl;
					return -1;
				}
		        delete handler;
		        delete parser;
		    } catch (const std::exception &e) {
		        std::cerr << "Caught a critical exception: " << e.what() << endl;
		        return -1;
		    } catch (...) {
		        std::cerr << "Caught a critical exception of unknown type!" << endl;
		        return -1;
		    }
		    return 0;
	}
	ref<Scene> m_scene;
	ref<RenderJob> m_thr;
	int m_argc;
	bool m_rendered;
};

class EMCAServer : public Utility {
public:
    int run(int argc, char **argv) {
        if (argc < 2) {
        	std::cout << "Need a scene.xml file ..." << std::endl;
            return 0;
        }
    	int port = 50013;
    	emca::RenderController *emcaController = new CopyMitsuba(argc, argv);
    	emcaController->setDataApi(emca::DataApiMitsuba::getInstance());
    	emcaServer = new emca::Server(port, emcaController);
    	emcaServer->run();
        return 0;
    }
    MTS_DECLARE_UTILITY()
};

MTS_EXPORT_UTILITY(EMCAServer, "Explorer of Monte-Carlo based Algorithms")
MTS_NAMESPACE_END
