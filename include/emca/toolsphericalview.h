/*
 * vsdtoolsphericalview.h
 *
 *  Created on: Feb 20, 2019
 *      Author: kreisl
 */

#ifndef INCLUDE_EMCA_VSDTOOLSPHERICALVIEW_H_
#define INCLUDE_EMCA_VSDTOOLSPHERICALVIEW_H_

#include <emca/platform.h>
#include <emca/tool.h>
#include <mitsuba/core/plugin.h>
#include <mitsuba/render/scene.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA SphericalView : public Tool {
public:

	SphericalView(std::string name, short id, bool *isCollecting);

	void setScene(const mitsuba::Scene *scene) { m_scene = scene; }

	void run();
	void serialize(Stream *stream);
	void deserialize(Stream *stream);

private:
	const mitsuba::Scene *m_scene = nullptr;
	mitsuba::Vector2i m_renderSize{256, 128};
	mitsuba::Point3f m_p{0.0, 0.0, 0.0};
	int m_numSamples = 16;
    std::string m_integratorName;
	fs::path m_path = fs::absolute("sphericalView.exr");
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDTOOLSPHERICALVIEW_H_ */
