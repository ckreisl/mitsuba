
#include <emca/rendercontroller.h>

EMCA_NAMESPACE_BEGIN

RenderController::RenderController() {
	m_renderInfo = new RenderInfo();
	m_dataapi = NULL;
}

RenderController::~RenderController() {
	delete m_renderInfo;
	delete m_dataapi;
}

void RenderController::readRenderInfo(Stream *stream) {
	//m_renderInfo = new RenderInfo(stream);
	m_renderInfo->deserialize(stream);
	updateRenderInfo(m_renderInfo);
}

void RenderController::respondRenderInfo(Stream *stream) {
	setRenderInformation();
	m_renderInfo->serialize(stream);
}

void RenderController::respondRenderImage(Stream *stream) {
	renderImage();
	stream->writeShort(Message::EMCA_RENDER_IMAGE);
	//stream->writeString(m_renderInfo->getOutputFilepath());
	//stream->writeString(m_renderInfo->getFileExtension());
}

void RenderController::respondSceneData(Stream *stream) {
	setCameraInformation(stream);
	std::cout << "Send Camera Information DONE!" << std::endl;
	setMeshInformation(stream);
	std::cout << "Send Mesh Information DONE!" << std::endl;
}

void RenderController::respondRenderData(Stream *stream) {
	m_dataapi->enable();
	int x = stream->readInt();
	int y = stream->readInt();
	renderPixel(x, y);
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

