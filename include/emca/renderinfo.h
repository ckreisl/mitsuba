/*
 * vsdsceneinfo.h
 *
 *  Created on: Aug 9, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_VSDRENDERINFO_H_
#define INCLUDE_EMCA_VSDRENDERINFO_H_

#include <emca/platform.h>
#include <emca/stream.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA RenderInfo {
public:

	RenderInfo();

	void setSceneName(std::string sceneName) { m_sceneName = sceneName; }
	void setOutputFilepath(std::string pathToOutputFile) { m_outputFilepath = pathToOutputFile; }
	void setOutputFileExtension(std::string extension) { m_outputFileExtension = extension; }
	void setSampleCount(int sampleCount) { m_sampleCount = sampleCount; }
	void setSampleCount(Stream *stream) { m_sampleCount = stream->readInt(); }

	std::string getSceneName() const { return m_sceneName; }
	std::string getOutputFilepath() const { return m_outputFilepath; }
	std::string getFileExtension() const { return m_outputFileExtension; }
	int getSampleCount() const { return m_sampleCount; }

	void serialize(Stream *stream);
	void deserialize(Stream *stream);

private:
	std::string m_sceneName;
	std::string m_outputFilepath;
	std::string m_outputFileExtension;
	int m_sampleCount;
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDRENDERINFO_H_ */
