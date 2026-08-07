/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:35:17 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 15:31:10 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/socket/Socket.hpp"

Socket::Socket() 
: _fd(-1), _server_index(-1), _type(CONNECTION), _http_request(HttpRequest()), _http_response(HttpResponse()) , _parent_index(-1)
{}

Socket::Socket(int fd, int server_index, Socket_type type, HttpRequest& request, HttpResponse& response) 
: _fd(fd), _server_index(server_index), _type(type), _http_request(request), _http_response(HttpResponse(response))
{}

Socket::Socket(const Socket& src) 
: _fd(src._fd), _server_index(src._server_index), _type(src._type), _http_request(src._http_request), _http_response(src._http_response), _parent_index(src._parent_index)
{}
Socket::~Socket() {}

int &Socket::getFd()
{
	return _fd;
}

int Socket::getFd() const
{
	return _fd;
}

int Socket::getServerIndex() const
{
	return _server_index;
}

Socket_type Socket::getType() const
{
	return _type;
}

void Socket::setFd(int fd)
{
	_fd = fd;
}

void Socket::setServerIndex(int index)
{
	_server_index = index;
}

void Socket::setType(Socket_type type)
{
	_type = type;
}

Socket& Socket::operator=(const Socket& src)
{
	if (this != &src)
	{
		_fd = src._fd;
		_server_index = src._server_index;
		_type = src._type;
	}
	return *this;
}
