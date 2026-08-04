/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:35:17 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/04 15:28:16 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/socket/socket.hpp"

Socket::Socket() 
: _fd(-1), _server_index(-1), _type(CONNECTION), _http_request(HttpRequest()), _http_response(HttpResponse()) , _parent_index(-1)
{}

Socket::Socket(int fd, int server_index, Socket_type type, HttpRequest& request, HttpResponse& response) 
: _fd(fd), _server_index(server_index), _type(type), _http_request(request), _http_response(HttpResponse(response))
{}

Socket::Socket(const Socket& other) 
: _fd(other._fd), _server_index(other._server_index), _type(other._type), _http_request(other._http_request), _http_response(other._http_response), _parent_index(other._parent_index)
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

HttpRequest& Socket::getHttpRequest() const
{
	return _http_request;
}

int Socket::getParentIndex() const
{
	return _parent_index;
}

void Socket::setParentIndex(int index)
{
	_parent_index = index;
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

void Socket::setHttpRequest(HttpRequest& request)
{
	_http_request = request;
}



Socket& Socket::operator=(const Socket& other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_server_index = other._server_index;
		_type = other._type;
		_http_request = other._http_request;
		_http_response = other._http_response;
		_parent_index = other._parent_index;
	}
	return *this;
}
