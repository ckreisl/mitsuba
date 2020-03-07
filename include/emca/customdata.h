/*
 * CustomData.h
 *
 *  Created on: Feb 25, 2019
 *      Author: kreisl
 */

#ifndef INCLUDE_EMCA_VSDCUSTOMDATA_H_
#define INCLUDE_EMCA_VSDCUSTOMDATA_H_

#include <emca/platform.h>
#include <emca/stream.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA CustomData {
public:
	CustomData(std::string name, short id) : m_name(name), m_index(id) { }
	virtual ~CustomData() { }

	// serialized the data
	virtual void serialize(Stream *stream) = 0;

	// get message size of serializing data
	virtual unsigned int getMsgLength() = 0;

	inline std::string getName() const { return m_name; }
	inline short getIndex() const { return m_index; }

private:
	std::string m_name;
	short m_index;
};


class MTS_EXPORT_EMCA TestCustomClass : public CustomData {
public:

	TestCustomClass() : CustomData("UserData", 256) { }
	~TestCustomClass() { }

	void serialize(Stream *stream) {
		stream->writeInt(x);
		stream->writeInt(y);
	}

	unsigned int getMsgLength() {
		return (unsigned int) 2*sizeof(int);
	}

private:

	int x = 4;
	int y = 2;

};


EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDCUSTOMDATA_H_ */

