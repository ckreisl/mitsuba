/*
    This file is part of Mitsuba, a physically based rendering system.

    Copyright (c) 2007-2014 by Wenzel Jakob and others.

    Mitsuba is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Mitsuba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    --------------------------------------------------------------------

    Minimalistic Stream Interface copied from mitsuba. Read description above.

    Edited for my Master-Thesis
    Kreisl, Christoph

    University of Tuebingen, Germany, 2018
*/

#pragma once
#ifndef INCLUDE_EMCA_STREAM_H
#define INCLUDE_EMCA_STREAM_H

#include <emca/platform.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA Stream {
public:

	Stream();
	virtual ~Stream() { }

	virtual void read(void *ptr, size_t size) = 0;
	virtual void write(const void *ptr, size_t size) = 0;

	/* write methods */
	void writeShort(short value);
	void writeUShort(unsigned short value);
	void writeInt(int value);
	void writeUInt(unsigned int value);
	void writeLong(long value);
	void writeULong(unsigned long value);
	void writeLongLong(long long value);
	void writeChar(char value);
	void writeUChar(unsigned char value);
	inline void writeBool(bool value) { writeUChar(value); }
	void writeFloat(float value);
	void writeDouble(double value);

	/* read methods */
	short readShort();
	unsigned short readUShort();
	int readInt();
	unsigned int readUInt();
	long readLong();
	unsigned long readULong();
	long long readLongLong();
	char readChar();
	unsigned char readUChar();
	inline bool readBool() { return static_cast<bool>(readUChar()); }
	float readFloat();
	double readDouble();

	/* specific write functions */
	std::string readString();
	void writeString(std::string value);
	void writeFloatArray(const float *data, size_t size);
	void writeIntArray(const int *data, size_t size);

	template <typename T> void writeArray(const T *array, size_t count);
	template <typename T, size_t N> inline void writeArray(const T (&arr)[N]) {
		writeArray(&arr[0], N);
	}

private:
	template <typename T>
	std::string toString(T number);
};

template<> inline void Stream::writeArray(const float *array, size_t count) { return writeFloatArray(array, count); }
template<> inline void Stream::writeArray(const int *array, size_t count) { return writeIntArray(array, count); }

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_STREAM_H */
