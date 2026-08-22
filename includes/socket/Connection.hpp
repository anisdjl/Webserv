/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anis <anis@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 20:03:14 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/22 14:05:46 by anis             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Socket.hpp"

class Connection : public Socket
{
    private:
        	HttpRequest		_http_request;
		    HttpResponse	_http_response;
    
    public:
        void setHttpRequest(HttpRequest& request)
		{
			this->_http_request = request;
		};
		void setHttpResponse(HttpResponse& response)
		{
			this->_http_response = response;
		};
		HttpRequest& getHttpRequest() 
		{
			return (this->_http_request);
		};
		HttpResponse& getHttpResponse() 
		{
			return (this->_http_response);
		};
        Connection() : Socket(), _http_request(HttpRequest()), _http_response(HttpResponse())
		{
			this->_type = CONNECTION;
		};
        Connection(int fd, int server_index, HttpRequest& request, HttpResponse& response):
		Socket(fd, server_index, CONNECTION), _http_request(request), _http_response(response)
		{};
        Connection(const Connection& src)
		 : Socket(src), _http_request(src._http_request), _http_response(src._http_response)
		{};
        ~Connection()
		{};
        Connection& operator=(const Connection& src)
		{
			if (this != &src)
			{
				this->_fd = src._fd;
				this->_server_index = src._server_index;
				this->_type = src._type;
				this->_http_request = src._http_request;
				this->_http_response = src._http_response;
			}
			return (*this);
		};
};

#endif