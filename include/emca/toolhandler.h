/*
 * toolhandler.h
 *
 *  Created on: Sep 1, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_VSDTOOLHANDLER_H_
#define INCLUDE_EMCA_VSDTOOLHANDLER_H_

#include <emca/platform.h>
#include <emca/tool.h>
#include <emca/stream.h>
#include <map>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA ToolHandler {
public:

	ToolHandler(bool *isCollecting);
	~ToolHandler();

	Tool* getToolByName(std::string name);
	Tool* getToolById(short id);

	void serialize(Stream *stream);

private:
	std::map<short, Tool*> m_tools;
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDTOOLHANDLER_H_ */
