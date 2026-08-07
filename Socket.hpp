/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 16:01:25 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 16:09:18 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../config/Config.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"

enum Socket_type
{
    LISTENER,
    CONNECTION,
    CGI
};

class Socket
{
	protected:
		int         	_fd;
    	int         	_server_index;
    	Socket_type 	_type;
		
		Socket() : _fd(-1), _server_index(-1), _type(LISTENER) {};
		Socket(int fd, int server_index, Socket_type type)
		: _fd(fd), _server_index(server_index), _type(type) 
		{};
		Socket(const Socket& src) 
		: _fd(src._fd), _server_index(src._server_index), _type(src._type) 
		{};
		
	public :

		
		~Socket();
		int &getFd()
		{
			return (this->_fd);
		};
		int getFd() const
		{
			return (this->_fd);
		};
		int getServerIndex() const
		{
			return (this->_server_index);
		};
		Socket_type getType() const
		{
			return (this->_type);
		};
		void setFd(int fd)
		{
			this->_fd = fd;
		};
		void setServerIndex(int index)
		{
			this->_server_index = index;
		};
		void setType(Socket_type type)
		{
			this->_type = type;
		};
		Socket& operator=(const Socket& src)
		{
			if (this != &src)
			{
				this->_fd = src._fd;
				this->_server_index = src._server_index;
				this->_type = src._type;
			}
			return (*this);
		};
};


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
		HttpRequest& getHttpRequest() const
		{
			return (this->_http_request);
		};
		HttpResponse& getHttpResponse() const
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