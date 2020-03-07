/*
 * vsdserver.h
 *
 *  Created on: Mar 13, 2018
 *      Author: kreisl
 */
#pragma once
#ifndef INCLUDE_EMCA_VSDSERVER_H_
#define INCLUDE_EMCA_VSDSERVER_H_

#include <emca/messages.h>
#include <emca/platform.h>
#include <emca/stream.h>
#include <emca/rendercontroller.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA Server {
public:

	typedef int socket_t;

	Server(int port, RenderController *controller);
	~Server();

	void run();
	void shutdown();

	inline int getPort() { return m_port; }
	socket_t getClientSocket() { return m_clientSocket; }
	socket_t getServerSocket() { return m_serverSocket; }

	Stream* getStream() { return m_stream; }
	RenderController* getController() { return m_controller; }

private:

	int m_port;
	short m_lastMsg;
	socket_t m_clientSocket;
	socket_t m_serverSocket;
	Stream *m_stream;
	RenderController *m_controller;
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDSERVER_H_ */
