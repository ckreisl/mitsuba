
/********************************************** MITSUBA TYPES **********************************************/

#include <emca/dataapimitsuba.h>
#include <emca/messages.h>

EMCA_NAMESPACE_BEGIN

DataApiMitsuba* DataApiMitsuba::m_ptrInstance = nullptr;

DataApiMitsuba *DataApiMitsuba::getInstance() {
	if(!m_ptrInstance)
		m_ptrInstance = new DataApiMitsuba;
	return m_ptrInstance;
}

void DataApiMitsuba::setPathOrigin(mitsuba::Point3f p) {
	DataApi::setPathOrigin((float)p.x, (float)p.y, (float)p.z);
}

void DataApiMitsuba::setIntersectionPos(mitsuba::Point3f p) {
	DataApi::setIntersectionPos((float)p.x, (float)p.y, (float)p.z);
}

void DataApiMitsuba::setNextEventEstimationPos(mitsuba::Point3f p, bool occluded) {
	DataApi::setNextEventEstimationPos((float)p.x, (float)p.y, (float)p.z, occluded);
}

void DataApiMitsuba::setIntersectionPosEnvmap(mitsuba::Point3f p) {
	DataApi::setIntersectionPosEnvmap((float)p.x, (float)p.y, (float)p.z);
}

void DataApiMitsuba::setIntersectionEstimate(mitsuba::Spectrum c, float alpha) {
	DataApi::setIntersectionEstimate((float)c[0], (float)c[1], (float)c[2], alpha);
}

void DataApiMitsuba::setFinalEstimate(mitsuba::Spectrum c, float alpha) {
	DataApi::setFinalEstimate((float)c[0], (float)c[1], (float)c[2], alpha);
}

void DataApiMitsuba::addVertexInfoPoint2i(std::string s, mitsuba::Point2i p) {
	DataApi::addVertexInfo(s, (int)p.x, (int)p.y);
}

void DataApiMitsuba::addVertexInfoPoint2f(std::string s, mitsuba::Point2f p) {
	DataApi::addVertexInfo(s, (float)p.x, (float)p.y);
}

void DataApiMitsuba::addVertexInfoPoint3i(std::string s, mitsuba::Point3i p) {
	DataApi::addVertexInfo(s, (int)p.x, (int)p.y, (int)p.z);
}

void DataApiMitsuba::addVertexInfoPoint3f(std::string s, mitsuba::Point3f p) {
	DataApi::addVertexInfo(s, (float)p.x, (float)p.y, (float)p.z);
}

void DataApiMitsuba::addVertexInfoVector2i(std::string s, mitsuba::Vector2i p) {
	DataApi::addVertexInfo(s, (int)p.x, (int)p.y);
}

void DataApiMitsuba::addVertexInfoVector2f(std::string s, mitsuba::Vector2f p) {
	DataApi::addVertexInfo(s, (float)p.x, (float)p.y);
}

void DataApiMitsuba::addVertexInfoVector3i(std::string s, mitsuba::Vector3i p) {
	DataApi::addVertexInfo(s, (int)p.x, (int)p.y, (int)p.z);
}

void DataApiMitsuba::addVertexInfoVector3f(std::string s, mitsuba::Vector3f p) {
	DataApi::addVertexInfo(s, (float)p.x, (float)p.y, (float)p.z);
}

void DataApiMitsuba::addVertexInfoSpectrum(std::string s, mitsuba::Spectrum c, float alpha) {
	DataApi::addVertexInfo(s, (float)c[0], (float)c[1], (float)c[2], alpha);
}

void DataApiMitsuba::addPathInfoPoint2i(std::string s, mitsuba::Point2i p) {
	DataApi::addPathInfo(s, (int)p.x, (int)p.y);
}

void DataApiMitsuba::addPathInfoPoint2f(std::string s, mitsuba::Point2f p) {
	DataApi::addPathInfo(s, (float)p.x, (float)p.y);
}

void DataApiMitsuba::addPathInfoPoint3i(std::string s, mitsuba::Point3i p) {
	DataApi::addPathInfo(s, (int)p.x, (int)p.y, (int)p.z);
}

void DataApiMitsuba::addPathInfoPoint3f(std::string s, mitsuba::Point3f p) {
	DataApi::addPathInfo(s, (float)p.x, (float)p.y, (float)p.z);
}

void DataApiMitsuba::addPathInfoVector2i(std::string s, mitsuba::Vector2i p) {
	DataApi::addPathInfo(s, (int)p.x, (int)p.y);
}

void DataApiMitsuba::addPathInfoVector2f(std::string s, mitsuba::Vector2f p) {
	DataApi::addPathInfo(s, (float)p.x, (float)p.y);
}

void DataApiMitsuba::addPathInfoVector3i(std::string s, mitsuba::Vector3i p) {
	DataApi::addPathInfo(s, (int)p.x, (int)p.y, (int)p.z);
}

void DataApiMitsuba::addPathInfoVector3f(std::string s, mitsuba::Vector3f p) {
	DataApi::addPathInfo(s, (float)p.x, (float)p.y, (float)p.z);
}
void DataApiMitsuba::addPathInfoSpectrum(std::string s, mitsuba::Spectrum c, float alpha) {
	DataApi::addPathInfo(s, (float)c[0], (float)c[1], (float)c[2], alpha);
}

EMCA_NAMESPACE_END

