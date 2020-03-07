/*
 * vsdvertexdata.h
 *
 *  Created on: Aug 13, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_VSDVERTEXDATA_H_
#define INCLUDE_EMCA_VSDVERTEXDATA_H_

#include <emca/platform.h>
#include <emca/stream.h>
#include <emca/userdata.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA VertexData : public UserData {
public:

	VertexData() ;
	VertexData(int depthIdx);
	~VertexData();

	void setIntersectionPos(Point3f pos);
	void setNextEventEstimationPos(Point3f pos, bool occluded);
	void setIntersectionPosEnvmap(Point3f pos);
	void setIntersectionEstimate(Color4f li);

	void serialize(Stream *stream);

private:
	int m_depthIdx;			/* current path depth */
	Point3f m_pos;			/* intersection point in world coordinates */
	Point3f m_posNE;		/* next event estimation point in world coordinates */
	Point3f m_posEnvmap;	/* is set if last ray intersects with envmap */
	Color4f m_estimate;		/* current computed estimate at this intersection */

	bool m_setPos;
	bool m_setNE;
	bool m_occludedNE;
	bool m_hitEnvmap;
	bool m_setEstimate;
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDVERTEXDATA_H_ */
