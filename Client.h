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

#if !defined(CLIENT_H)
#define CLIENT_H

#include "Socket.h"

#include <string>

namespace libnetrom
{
	class Client
	{
		public:
			Client(const std::string& interface);
			~Client();

			bool connect(const std::string& callsign);

			Socket* connected();

			int  getErrno() const;

			void close();

		private:
			std::string m_interface;
			std::string m_callsign;
			int         m_socket;
			int         m_errno;
	};
};

#endif

