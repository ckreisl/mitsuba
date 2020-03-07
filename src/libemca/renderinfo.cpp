
#include <emca/renderinfo.h>
#include <emca/messages.h>

EMCA_NAMESPACE_BEGIN

RenderInfo::RenderInfo() {
	// default values
	m_sceneName = "";
	m_outputFilepath = "";
	m_outputFileExtension = "";
	m_sampleCount = -1;
}

void RenderInfo::serialize(Stream *stream) {
	stream->writeShort(Message::EMCA_HEADER_RENDER_INFO);
	stream->writeString(m_sceneName);
	stream->writeString(m_outputFilepath);
	stream->writeString(m_outputFileExtension);
	stream->writeInt(m_sampleCount);
}

void RenderInfo::deserialize(Stream *stream) {
	this->m_sampleCount = stream->readInt();
}

EMCA_NAMESPACE_END


