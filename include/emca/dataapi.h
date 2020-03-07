#pragma once
#if !defined(INCLUDE_EMCA_DATAAPI_H)
#define INCLUDE_EMCA_DATAAPI_H

#include <mutex>
#include <emca/platform.h>
#include <emca/stream.h>
#include <emca/pathdata.h>
#include <emca/toolhandler.h>
#include <emca/customdata.h>

/**
 * Singleton Pattern
 */

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA DataApi {
public:

	DataApi() { m_isCollecting = false; };
	virtual ~DataApi() { delete th; };

	void setSampleIdx(int sampleIdx);
	void setDepthIdx(int depthIdx);

	void setPathOrigin(float x, float y, float z);
	void setIntersectionPos(float x, float y, float z);
	void setNextEventEstimationPos(float x, float y, float z, bool occluded);
	void setIntersectionPosEnvmap(float x, float y, float z);
	void setIntersectionEstimate(float r, float g, float b, float alpha = 1.0);

	void setFinalEstimate(float r, float g, float b, float alpha = 1.0);

	void addVertexInfo(std::string s, bool val);
	void addVertexInfo(std::string s, float val);
	void addVertexInfo(std::string s, double val);
	void addVertexInfo(std::string s, int val);
	void addVertexInfo(std::string s, int x, int y);
	void addVertexInfo(std::string s, float x, float y);
	void addVertexInfo(std::string s, int x, int y, int z);
	void addVertexInfo(std::string s, float x, float y, float z);
	void addVertexInfo(std::string s, float r, float g, float b, float alpha);
    void addVertexInfo(std::string s, std::string val);
	void addVertexCustomData(CustomData *data);

	void addPathInfo(std::string s, bool val);
	void addPathInfo(std::string s, float val);
	void addPathInfo(std::string s, double val);
	void addPathInfo(std::string s, int val);
	void addPathInfo(std::string s, int x, int y);
	void addPathInfo(std::string s, float x, float y);
	void addPathInfo(std::string s, int x, int y, int z);
	void addPathInfo(std::string s, float x, float y, float z);
	void addPathInfo(std::string s, float r, float g, float b, float alpha);
    void addPathInfo(std::string s, std::string val);
	void addPathCustomData(CustomData *data);

	void serialize(Stream *stream);

	void clear();
	void enable();
	void disable();

	inline Tool* getToolByName(std::string name) { return th->getToolByName(name); }
	inline Tool* getToolById(short id) { return th->getToolById(id); }

	inline bool isCollecting() { return m_isCollecting; }

private:
	std::mutex m_mutex;
	bool m_isCollecting;
	int m_currentSampleIdx = -1;
	int m_currentDepthIdx = -1;
	std::map<int, PathData> m_paths;

	ToolHandler *th = new ToolHandler(&m_isCollecting);
};

EMCA_NAMESPACE_END

#endif // INCLUDE_EMCA_DATAAPI_H
