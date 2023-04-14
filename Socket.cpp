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

#include "Socket.h"

#include <sys/select.h>
#include <unistd.h>

#include <cassert>

namespace libnetrom
{
	Socket::Socket(int socket, const std::string& peer) :
	m_socket(socket),
	m_peer(peer),
	m_errno(0)
	{
		assert(socket >= 0);
		assert(!peer.empty());
	}
	
	Socket::~Socket()
	{
	}

	std::string Socket::getPeer() const
	{
		return m_peer;
	}

	int Socket::read(uint8_t* data, int length)
	{
		assert(m_socket >= 0);
		assert(data != nullptr);
		assert(length > 0);

		struct timeval tv;
		tv.tv_sec  = 0;
		tv.tv_usec = 0;

		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(m_socket, &fds);
		
		int n = ::select(m_socket + 1, &fds, nullptr, nullptr, &tv);
		if (n < 0)
		{
			m_errno = errno;
			return -2;
		}
		else if (n == 0)
		{
			return -1;
		}
		else
		{
			n = ::read(m_socket, data, length);
			if (n < 0)
			{
				m_errno = errno;
				return -2;
			}
			else
			{
				return n;
			}
		}
	}
	
	bool Socket::write(const uint8_t* data, int length)
	{
		assert(m_socket >= 0);
		assert(data != nullptr);
		assert(length > 0);

		int offset = 0;
		while (length > 0)
		{
			if (length > MAX_PACKET_LEN)
			{
				int n = ::write(m_socket, data + offset, MAX_PACKET_LEN);
				if (n != MAX_PACKET_LEN)
				{
					m_errno = errno;
					return false;
				}
			}
			else
			{
				int n = ::write(m_socket, data + offset, length);
				if (n != length)
				{
					m_errno = errno;
					return false;
				}
			}

			offset += MAX_PACKET_LEN;
			length -= MAX_PACKET_LEN;
		}

		return true;
	}

	int Socket::getErrno() const
	{
		return m_errno;
	}

	void Socket::close()
	{
		assert(m_socket >= 0);
		
		::close(m_socket);
		m_socket = -1;
	}
};

