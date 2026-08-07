/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 14:59:47 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 15:29:54 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket/Connection.hpp"


Connection::Connection() : Socket(), _http_request(HttpRequest()), _http_response(HttpResponse()) 
{
	this->_type = CONNECTION;
}

Connection::Connection(int fd, int server_index, HttpRequest& request, HttpResponse& response)
: Socket(fd, server_index, CONNECTION, request, response), _http_request(request), _http_response(response) 
{}

Connection::Connection(const Connection& other) 
: Socket(other), _http_request(other._http_request), _http_response(other._http_response) 
{}

~Connection::~Connection() {}.


HttpRequest& Socket::getHttpRequest() const
{
	return this->_http_request;
}

HttpResponse& Socket::getHttpResponse() const
{
	return this->_http_response;
}

void Connection::setHttpRequest(HttpRequest& request)
{
	this->_http_request = request;
}

void Connection::setHttpResponse(HttpResponse& response)
{
	this->_http_response = response;
}
