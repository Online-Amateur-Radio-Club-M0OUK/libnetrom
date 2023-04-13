/*
 *   Copyright (C) 2022 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#if !defined(SOCKET_H)
#define SOCKET_H

#include "netrom.h"

#include <string>

namespace libnetrom
{
	class Socket
	{
		public:
			Socket(int socket, const std::string& peer);
			~Socket();

			std::string getPeer() const;

			int  read(uint8_t* data, int length);
			bool write(const uint8_t* data, int length);

			int  getErrno() const;

			void close();

		private:
			int         m_socket;
			std::string m_peer;
			int         m_errno;
	};
};

#endif

