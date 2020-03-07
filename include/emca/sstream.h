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

#pragma once
#ifndef INCLUDE_EMCA_SSTREAM_H_
#define INCLUDE_EMCA_SSTREAM_H_

#include <emca/platform.h>
#include <emca/stream.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA SocketStream : public Stream {
public:
	typedef int socket_t;

	SocketStream(socket_t socket);
	void write(const void *ptr, size_t size);
	void read(void *ptr, size_t size);

protected:
	virtual ~SocketStream();

private:
	socket_t m_clientSocket;
};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_SSTREAM_H_ */
