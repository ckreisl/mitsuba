/*
 * vsdinterface.h
 *
 *  Created on: Aug 9, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_RENDERCONTROLLER_H_
#define INCLUDE_EMCA_RENDERCONTROLLER_H_

#include <emca/dataapi.h>
#include <emca/messages.h>
#include <emca/platform.h>
#include <emca/renderinfo.h>
#include <emca/camera.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA RenderController {
public:

	RenderController();
	virtual ~RenderController();

	virtual void renderImage() = 0;
	virtual void renderPixel(
			unsigned int x,
			unsigned int y, int
			sampleCount) = 0;
	virtual void sendRenderInformation(Stream *stream) = 0;
	virtual void sendCameraData(Stream *stream) = 0;
	virtual void sendMeshData(Stream *stream) = 0;
	virtual void updateSampleCount(int sampleCount) = 0;

	inline DataApi *getDataApi() { return m_dataapi; }
	inline void setDataApi(DataApi* dataapi) { m_dataapi = dataapi; }

	void readRenderInfo(Stream *stream);
	void respondRenderInfo(Stream *stream);
	void respondRenderImage(Stream *stream);
	void respondSceneData(Stream *stream);
	void respondRenderData(Stream *stream);
	bool respondToolRequest(short id, Stream *stream);

protected:

private:
	DataApi *m_dataapi;
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_RENDERCONTROLLER_H_ */
