/*
 * vsdtool.h
 *
 *  Created on: Sep 1, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_VSDTOOL_H_
#define INCLUDE_EMCA_VSDTOOL_H_

#include <emca/platform.h>
#include <emca/stream.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA Tool {
public:

	Tool(std::string name, short id, bool *isCollecting) {
		m_name = name;
		m_id = id;
		m_isCollecting = isCollecting;
	}

	virtual ~Tool() { delete m_isCollecting; }

	std::string getName() const { return m_name; }
	short getId() const { return m_id; }

	virtual void run() = 0;
	virtual void serialize(Stream *stream) = 0;
	virtual void deserialize(Stream *stream) = 0;

protected:
	bool *m_isCollecting;
	short m_id;
	std::string m_name;

private:

};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDTOOL_H_ */
