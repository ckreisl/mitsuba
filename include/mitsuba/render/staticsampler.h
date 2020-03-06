/*
 * samplerseedmap.h
 *
 *  Created on: Feb 27, 2020
 *      Author: ckreisl
 */

#pragma once
#if !defined(__MITSUBA_DETERMINISTIC_PIXEL_SAMPLER_H_)
#define __MITSUBA_DETERMINISTIC_PIXEL_SAMPLER_H_
#include <mitsuba/render/sampler.h>

MTS_NAMESPACE_BEGIN

class MTS_EXPORT_RENDER StaticDeterministicPixelSampler {
public:

	static ref<Sampler> getSamplerToPixel(
			const int &x,
			const int &y,
			const int &sampleCount);

private:
	StaticDeterministicPixelSampler() { }
	StaticDeterministicPixelSampler(StaticDeterministicPixelSampler const&);
	StaticDeterministicPixelSampler& operator=(StaticDeterministicPixelSampler const&);
};

MTS_NAMESPACE_END

#endif /* __MITSUBA_DETERMINISTIC_PIXEL_SAMPLER_H_ */
