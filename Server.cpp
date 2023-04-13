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

#include "Server.h"

#include <unistd.h>

#include <sys/socket.h>

#include <netax25/ax25.h>
#include <netax25/axlib.h>

#include <cstring>
#include <cassert>

namespace libnetrom
{
	Server::Server(const std::string& interface) :
	m_interface(interface),
	m_socket(-1),
	m_errno(0)
	{
		assert(!interface.empty());
	}
	
	Server::~Server()
	{
	}

	bool Server::bind()
	{
		assert(m_socket == -1);

		m_socket = ::socket(AF_NETROM, SOCK_SEQPACKET | SOCK_NONBLOCK, 0);
		if (m_socket < 0)
		{
			m_errno  = errno;
			m_socket = -1;
			return false;
		}

		struct sockaddr_ax25 sockaddr;
		::memset(&sockaddr, 0x00, sizeof(struct sockaddr_ax25));
		sockaddr.sax25_family = AF_NETROM;
		ax25_aton_entry(m_interface.c_str(), sockaddr.sax25_call.ax25_call);
		sockaddr.sax25_ndigis = 0;

		int ret = ::bind(m_socket, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr_ax25));
		if (ret < 0)
		{
			m_errno  = errno;

			::close(m_socket);
			m_socket = -1;

			return false;
		}

		::listen(m_socket, 4);

		return true;
	}
	
	Socket* Server::accept()
	{
		assert(m_socket >= 0);

		struct timeval timeval;
		timeval.tv_sec  = 0;
		timeval.tv_usec = 0;

		fd_set fd_set;
		FD_ZERO(&fd_set);
		FD_SET(m_socket, &fd_set);
		
		int n = ::select(m_socket + 1, &fd_set, nullptr, nullptr, &timeval);
		if (n < 0)
		{
			m_errno = errno;
			return nullptr;
		}
		else if (n == 0)
		{
			m_errno = 0;
			return nullptr;
		}
		else
		{
			struct sockaddr_ax25 sockaddr;
			socklen_t socklen = 0;

			int s = ::accept(m_socket, (struct sockaddr*)&sockaddr, &socklen);
			if (s < 0)
			{
				m_errno = errno;
				return nullptr;
			}
			else
			{
				return new Socket(s, std::string(ax25_ntoa(&sockaddr.sax25_call)));
			}
		}
	}

	int Server::getErrno() const
	{
		return m_errno;
	}

	void Server::close()
	{
		assert(m_socket >= 0);
		
		::close(m_socket);
		m_socket = -1;
	}
};

