#pragma once
#if !defined(INCLUDE_EMCA_DATAAPIMITSUBA_H)
#define INCLUDE_EMCA_DATAAPIMITSUBA_H

#include <mitsuba/mitsuba.h>
#include <emca/dataapi.h>

MTS_NAMESPACE_BEGIN

/**
 * Mitsuba Type Interface
 */
class MTS_EXPORT_CORE DataApiMitsuba : public emca::DataApi {
public:

	static DataApiMitsuba *getInstance();

	// overwrite functions with own data types
	void setPathOrigin(mitsuba::Point3f p);
	void setIntersectionPos(mitsuba::Point3f p);
	void setNextEventEstimationPos(mitsuba::Point3f p, bool occluded);
	void setIntersectionPosEnvmap(mitsuba::Point3f p);
	void setIntersectionEstimate(mitsuba::Spectrum c, float alpha = 1.0);
	void setFinalEstimate(mitsuba::Spectrum c, float alpha = 1.0);

	void addVertexInfoPoint2i(std::string s, mitsuba::Point2i p);
	void addVertexInfoPoint2f(std::string s, mitsuba::Point2f p);
	void addVertexInfoPoint3i(std::string s, mitsuba::Point3i p);
	void addVertexInfoPoint3f(std::string s, mitsuba::Point3f p);
	void addVertexInfoVector2i(std::string s, mitsuba::Vector2i p);
	void addVertexInfoVector2f(std::string s, mitsuba::Vector2f p);
	void addVertexInfoVector3i(std::string s, mitsuba::Vector3i p);
	void addVertexInfoVector3f(std::string s, mitsuba::Vector3f p);
	void addVertexInfoSpectrum(std::string s, mitsuba::Spectrum c, float alpha = 1.0);

	void addPathInfoPoint2i(std::string s, mitsuba::Point2i p);
	void addPathInfoPoint2f(std::string s, mitsuba::Point2f p);
	void addPathInfoPoint3i(std::string s, mitsuba::Point3i p);
	void addPathInfoPoint3f(std::string s, mitsuba::Point3f p);
	void addPathInfoVector2i(std::string s, mitsuba::Vector2i p);
	void addPathInfoVector2f(std::string s, mitsuba::Vector2f p);
	void addPathInfoVector3i(std::string s, mitsuba::Vector3i p);
	void addPathInfoVector3f(std::string s, mitsuba::Vector3f p);
	void addPathInfoSpectrum(std::string s, mitsuba::Spectrum c, float alpha = 1.0);

private:
	DataApiMitsuba() : emca::DataApi() { }
	DataApiMitsuba(DataApiMitsuba const&) = delete;
	~DataApiMitsuba() { }
	void operator=(const DataApiMitsuba &) = delete;
	static DataApiMitsuba *m_ptrInstance;
};

MTS_NAMESPACE_END

#endif // INCLUDE_EMCA_DATAAPIMITSUBA_H
