
#include <emca/rendercontroller.h>

EMCA_NAMESPACE_BEGIN

RenderController::RenderController() {
	m_dataapi = NULL;
}

RenderController::~RenderController() {
	delete m_dataapi;
}

void RenderController::readRenderInfo(Stream *stream) {
	RenderInfo renderInfo;
	renderInfo.deserialize(stream);
	updateSampleCount(renderInfo.getSampleCount());
}

void RenderController::respondRenderInfo(Stream *stream) {
	sendRenderInformation(stream);
}

void RenderController::respondRenderImage(Stream *stream) {
	renderImage();
	stream->writeShort(Message::EMCA_RENDER_IMAGE);
}

void RenderController::respondSceneData(Stream *stream) {
	sendCameraData(stream);
	std::cout << "Send Camera Information DONE!" << std::endl;
	sendMeshData(stream);
	std::cout << "Send Mesh Information DONE!" << std::endl;
}

void RenderController::respondRenderData(Stream *stream) {
	m_dataapi->enable();
	int x = stream->readInt();
	int y = stream->readInt();
	int sampleCount = stream->readInt();
	renderPixel(x, y, sampleCount);
	m_dataapi->serialize(stream);
	m_dataapi->disable();
	m_dataapi->clear();
}

bool RenderController::respondToolRequest(short id, Stream *stream) {
	Tool* tool = m_dataapi->getToolById(id);
	if(!tool) return false;
	tool->deserialize(stream);
	bool finished = false;
	try {
		tool->run();
		finished = true;
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	if (finished) {
		tool->serialize(stream);
		return true;
	}

	return false;
}

EMCA_NAMESPACE_END

