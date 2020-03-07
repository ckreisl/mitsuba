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

    Minimalistic SocketStream Interface copied from mitsuba. Read description above.

    Edited for my Master-Thesis
    Kreisl, Christoph

    University of Tuebingen, Germany, 2018
*/

#include <cstddef>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include <emca/sstream.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR   -1

EMCA_NAMESPACE_BEGIN

SocketStream::SocketStream(socket_t socket) : m_clientSocket(socket) { }

SocketStream::~SocketStream() {
	if(close(m_clientSocket))
		std::cout << "Error by closing socket" << std::endl;
}

void SocketStream::read(void *ptr, size_t size) {
	char *data = (char *) ptr;
	while(size > 0) {
		ssize_t n = recv(m_clientSocket, data, size, 0);

		if(n == 0) {
			// TODO throw exception
		} else if(n == SOCKET_ERROR) {
			// TODO throw exception
		}

		size -= n;
		data += n;
	}
}

void SocketStream::write(const void *ptr, size_t size) {
	char *data = (char *) ptr;
	while(size > 0) {
		ssize_t n = send(m_clientSocket, data, size, MSG_NOSIGNAL);

		if (n == SOCKET_ERROR) {
			// TODO throw exception
		}

		size -= n;
		data += n;
	}
}

EMCA_NAMESPACE_END
