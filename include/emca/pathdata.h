/*
 * vsdpathdata.h
 *
 *  Created on: Aug 13, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_VSDPATHDATA_H_
#define INCLUDE_EMCA_VSDPATHDATA_H_

#include <emca/platform.h>
#include <emca/userdata.h>
#include <emca/stream.h>
#include <emca/vertexdata.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA PathData : public UserData {
public:

	PathData();
	PathData(int sampleIdx);
	~PathData();

	void setDepthIdx(int depthIdx);
	void setIntersectionPos(int depthIdx, Point3f pos);
	void setNextEventEstimationPos(int depthIdx, Point3f pos, bool occluded);
	void setIntersectionPosEnvmap(int depthIdx, Point3f pos);
	void setIntersectionEstimate(int depthIdx, Color4f li);

	void setPathOrigin(Point3f origin);
	void setFinalEstimate(Color4f li);

	void addVertexInfo(int depthIdx, std::string s, bool val);
	void addVertexInfo(int depthIdx, std::string s, float val);
	void addVertexInfo(int depthIdx, std::string s, double val);
	void addVertexInfo(int depthIdx, std::string s, int val);
	void addVertexInfo(int depthIdx, std::string s, int x, int y);
	void addVertexInfo(int depthIdx, std::string s, float x, float y);
	void addVertexInfo(int depthIdx, std::string s, int x, int y, int z);
	void addVertexInfo(int depthIdx, std::string s, float x, float y, float z);
	void addVertexInfo(int depthIdx, std::string s, float r, float g, float b, float alpha);
    void addVertexInfo(int depthIdx, std::string s, std::string val);

	void serialize(Stream *stream);
	void clear();

private:
	int m_sampleIdx; 						/* Current sample index */
	int m_pathDepth; 						/* Path length, amount of Intersections */
	Point3f m_pathOrigin;					/* Path origin */
	Color4f m_finalEstimate;				/* final light estimation of path */
	std::map<int, VertexData> m_segments;	/* data dictionary about each segment (intersection) */

	bool m_setFinalEstimate;
	bool m_visualizePath;
	bool m_visualizeNE;
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDPATHDATA_H_ */
