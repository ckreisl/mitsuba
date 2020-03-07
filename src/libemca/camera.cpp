

#include <emca/camera.h>
#include <emca/messages.h>

EMCA_NAMESPACE_BEGIN

Camera::Camera() {
	m_nearClip = 0.0;
	m_farClip = 100.0;
	m_focusDist = 7.0;
	m_fov = 39.0;
	m_up = Vec3f(0, 0, 1);
	m_dir = Vec3f(0, 1, 0);
	m_origin = Point3f(0, 0, 0);
}

Camera::~Camera() { }

void Camera::serialize(Stream *stream) {
	stream->writeShort(Message::EMCA_HEADER_CAMERA);
	stream->writeFloat(m_nearClip);
	stream->writeFloat(m_farClip);
	stream->writeFloat(m_focusDist);
	stream->writeFloat(m_fov);
	m_up.serialize(stream);
	m_dir.serialize(stream);
	m_origin.serialize(stream);
}


EMCA_NAMESPACE_END


