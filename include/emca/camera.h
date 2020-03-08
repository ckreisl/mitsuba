/*
 * vsdcamera.h
 *
 *  Created on: Aug 9, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_VSDCAMERA_H_
#define INCLUDE_EMCA_VSDCAMERA_H_

#include <emca/datatypes.h>
#include <emca/platform.h>
#include <emca/stream.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA Camera {
public:

	Camera();
	Camera(float nearClip, float farClip, float focusDist, float fov,
			Vec3f up, Vec3f dir, Point3f origin);
	~Camera();

	void setNearClip(float nearClip) { m_nearClip = nearClip; }
	void setFarClip(float farClip) { m_farClip = farClip; }
	void setFocusDist(float focusDist) { m_focusDist = focusDist; }
	void setFov(float fov) { m_fov = fov; }
	void setUpVector(Vec3f up) { m_up = up; }
	void setDirectionVector(Vec3f dir) { m_dir = dir; }
	void setOrigin(Point3f origin) { m_origin = origin; }

	void serialize(Stream *stream);

private:
	float m_nearClip;
	float m_farClip;
	float m_focusDist;
	float m_fov;
	Vec3f m_up;
	Vec3f m_dir;
	Point3f m_origin;
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDCAMERA_H_ */
