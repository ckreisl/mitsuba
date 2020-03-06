/*
 * samplerseedmap.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: ckreisl
 */

#include <mitsuba/render/staticsampler.h>
#include <mitsuba/core/properties.h>
#include <mitsuba/core/plugin.h>

MTS_NAMESPACE_BEGIN

ref<Sampler> StaticDeterministicPixelSampler::getSamplerToPixel(
		const int &x,
		const int &y,
		const int &sampleCount) {

	auto deterministicSeed = [](const int &x, const int &y) {
		int retval = x*x-2*y;
		return retval > 0 ? retval : -retval;
	};

	int seed = deterministicSeed(x, y);

	Properties props = Properties("deterministic");
	props.setInteger("sampleCount", sampleCount);
	props.setInteger("seed", seed);
	ref<Sampler> pixelSampler = static_cast<Sampler* >(PluginManager::getInstance()->
			createObject(MTS_CLASS(Sampler), props));
	pixelSampler->configure();
	return pixelSampler;
}

MTS_NAMESPACE_END



