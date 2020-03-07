
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <emca/server.h>
#include <emca/sstream.h>

#define INVALID_SOCKET -1

EMCA_NAMESPACE_BEGIN

Server::Server(int port, RenderController *controller) {
	m_port = port;
	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(m_serverSocket == INVALID_SOCKET) {
		// TODO throw exception invalid socket!
		std::cout << "Invalid Socket" << std::endl;
		exit(-1);
	}

	// default values
	m_lastMsg = -1;
	m_clientSocket = -1;
	m_stream = nullptr;

	m_controller = controller;
}

Server::~Server() {
	delete m_stream;
	delete m_controller;
}

void Server::shutdown() {
	try {
		close(m_clientSocket);
		close(m_serverSocket);
	} catch(std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}

}

void Server::run() {

	struct sockaddr_in server_addr, client_addr;
	bzero((char *) &server_addr, sizeof(server_addr));

	int option = 1;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(m_port);

	if(setsockopt(m_serverSocket, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char*)&option, sizeof(option)) < 0) {
		std::cout << "Server setsockopt failed" << std::endl;
		close(m_serverSocket);
		exit(2);
	}

	int isBind = bind(m_serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if(isBind < 0) {
		std::cout << "Server bind failed" << std::endl;
		close(m_serverSocket);
		exit(1);
	}

	LISTEN:int isListen = listen(m_serverSocket, 5);

	if(isListen < 0)
		exit(-1);

	std::cout << "Server is listening for connections ..." << std::endl;

	socklen_t len = sizeof(client_addr);
	bool running = true;

	while (running) {
		m_clientSocket = accept(m_serverSocket,
				(struct sockaddr*) (&client_addr), &len);
		if (m_clientSocket < 0)
			std::cout << "Client Socket Error" << std::endl;

		m_stream = new SocketStream(m_clientSocket);
		m_stream->writeShort(Message::EMCA_HELLO);
		m_lastMsg = m_stream->readShort();

		if (m_lastMsg != Message::EMCA_HELLO) {
			running = false;
			break;
		}

		std::cout << "Handshake complete! Starting data transfer ..." << std::endl;

		try {
			while(running) {
				// read header of message
				m_lastMsg = m_stream->readShort();
				std::cout << "Received header msg = " << m_lastMsg << std::endl;

				// check if incoming message is a tool
				if(m_controller->respondToolRequest(m_lastMsg, m_stream))
					continue;

				switch(m_lastMsg) {
				case Message::EMCA_HEADER_RENDER_INFO:
					std::cout << "Respond render info msg" << std::endl;
					m_controller->respondRenderInfo(m_stream);
					break;
				case Message::EMCA_SET_RENDER_INFO:
					std::cout << "Set render info msg" << std::endl;
					m_controller->readRenderInfo(m_stream);
					break;
				case Message::EMCA_HEADER_SCENE_DATA:
					std::cout << "Respond scene data msg" << std::endl;
					m_controller->respondSceneData(m_stream);
					break;
				case Message::EMCA_RENDER_IMAGE:
					std::cout << "Render image msg" << std::endl;
					m_controller->respondRenderImage(m_stream);
					break;
				case Message::EMCA_RENDER_PIXEL:
					std::cout << "Render pixel msg" << std::endl;
					m_controller->respondRenderData(m_stream);
					break;
				case Message::EMCA_DISCONNECT:
					std::cout << "Disconnect msg" << std::endl;
					m_stream->writeShort(Message::EMCA_DISCONNECT);
					close(m_clientSocket);
					running = false;
					break;
				case Message::EMCA_QUIT:
					std::cout << "Quit message!" << std::endl;
					m_stream->writeShort(Message::EMCA_QUIT);
					//shutdown(m_clientSocket, SHUT_RDWR);
					//shutdown(m_serverSocket, SHUT_RDWR);
					close(m_clientSocket);
					close(m_serverSocket);
					running = false;
					break;
				default:
					std::cout << "Unknown message received!" << std::endl;
					break;
				}
			}
		} catch (std::exception &e) {
			// TODO handle exception
            std::cerr << "caught exception: " << e.what() << std::endl;

            m_lastMsg = Message::EMCA_DISCONNECT;
            m_stream->writeShort(Message::EMCA_DISCONNECT);
            close(m_clientSocket);
            running = false;
		} catch (...) {
			// TODO handle exception
            std::cerr << "caught exception." << std::endl;

            m_lastMsg = Message::EMCA_DISCONNECT;
            m_stream->writeShort(Message::EMCA_DISCONNECT);
            close(m_clientSocket);
            running = false;
		}

		// TODO remove goto command and go for a clean implementation if client disconnects.
		if(m_lastMsg == Message::EMCA_DISCONNECT)
			goto LISTEN;
	}
}

EMCA_NAMESPACE_END



