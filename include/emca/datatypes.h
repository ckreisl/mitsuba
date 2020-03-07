/*
 * vsddatatypes.h
 *
 *  Created on: Aug 9, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_VSDDATATYPES_H_
#define INCLUDE_EMCA_VSDDATATYPES_H_

#include <emca/platform.h>
#include <emca/stream.h>

EMCA_NAMESPACE_BEGIN

template <typename T> struct MTS_EXPORT_EMCA TPoint2 {
	const static int dim = 2;
	T p[dim];

	TPoint2() { p[0] = 0; p[1] = 0; }
	TPoint2(T x, T y) { p[0] = x; p[1] = y; }

	T x() const { return p[0]; }
	T y() const { return p[1]; }

	void serialize(Stream *stream) {
		stream->writeArray(p, dim);
	}

};

template <typename T> struct MTS_EXPORT_EMCA TPoint3 {
	const static int dim = 3;
	T p[dim];

	TPoint3() { p[0] = 0; p[1] = 0; p[2] = 0; }
	TPoint3(T x, T y, T z) { p[0] = x; p[1] = y; p[2] = z; }

	T x() const { return p[0]; }
	T y() const { return p[1]; }
	T z() const { return p[2]; }

	void serialize(Stream *stream) {
		stream->writeArray(p, dim);
	}
};

template <typename T> struct MTS_EXPORT_EMCA TVec2 {
	const static int dim = 2;
	T p[dim];

	TVec2() { p[0] = 0; p[1] = 0; }
	TVec2(T x, T y) { p[0] = x; p[1] = y; }

	T x() const { return p[0]; }
	T y() const { return p[1]; }

	void serialize(Stream *stream) {
		stream->writeArray(p, dim);
	}

};

template <typename T> struct MTS_EXPORT_EMCA TVec3 {
	const static int dim = 3;
	T p[dim];

	TVec3() { p[0] = 0; p[1] = 0; p[2] = 0; }
	TVec3(T x, T y, T z) { p[0] = x; p[1] = y; p[2] = z; }

	T x() const { return p[0]; }
	T y() const { return p[1]; }
	T z() const { return p[2]; }

	void serialize(Stream *stream) {
		stream->writeArray(p, dim);
	}
};

template <typename T> struct MTS_EXPORT_EMCA TColor4 {
	const static int dim = 4;
	T c[dim];

	TColor4() { c[0] = 0; c[1] = 0; c[2] = 0; c[3] = 0; }
	TColor4(T x, T y, T z) { c[0] = x; c[1] = y; c[2] = z; c[3] = 1; }
	TColor4(T x, T y, T z, T alpha) { c[0] = x; c[1] = y; c[2] = z; c[3] = alpha; }

	T x() const { return c[0]; }
	T y() const { return c[1]; }
	T z() const { return c[2]; }
	T alpha() const { return c[3]; }

	void serialize(Stream *stream) {
		stream->writeArray(c, dim);
	}
};

typedef TPoint2<float> 	Point2f;
typedef TPoint2<int> 	Point2i;
typedef TPoint3<float>	Point3f;
typedef TPoint3<int>	Point3i;
typedef TVec2<float>	Vec2f;
typedef TVec2<int>		Vec2i;
typedef TVec3<float>	Vec3f;
typedef TVec3<int>		Vec3i;
typedef TColor4<float>	Color4f;

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDDATATYPES_H_ */
