/*
 * vsdtoolhandler.cpp
 *
 *  Created on: Sep 1, 2018
 *      Author: ckreisl
 */

#include <emca/toolhandler.h>
#include <emca/toolsphericalview.h>

EMCA_NAMESPACE_BEGIN

ToolHandler::ToolHandler(bool *isCollecting) {
	/* add tools here */
	Tool *sphericalView = new SphericalView("SphericalView", 66, isCollecting);
	m_tools.insert(std::make_pair(sphericalView->getId(), sphericalView));
}

ToolHandler::~ToolHandler() {
	std::map<short, Tool*>::iterator it;
	for(it = m_tools.begin(); it != m_tools.end(); it++) {
		delete it->second;
	}
}

Tool* ToolHandler::getToolByName(std::string name) {
	std::map<short, Tool*>::iterator it;
	for(it = m_tools.begin(); it != m_tools.end(); it++) {
		if(it->second->getName() == name)
			return it->second;
	}
	return NULL;
}

Tool* ToolHandler::getToolById(short id) {
	return m_tools.find(id) == m_tools.end() ? NULL : m_tools.find(id)->second;
}

void ToolHandler::serialize(Stream *stream) {
	std::map<short, Tool*>::iterator it;
	for(it = m_tools.begin(); it != m_tools.end(); it++)
		it->second->serialize(stream);
}

EMCA_NAMESPACE_END


