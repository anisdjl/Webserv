/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjelili <adjelili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 17:39:24 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/18 12:40:19 by adjelili         ###   ########.fr       */
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

		
		virtual ~Socket();
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


bool ft_treat_socket(std::map<int, Socket *> &map_socket, struct epoll_event &event, Config *config, const int &epollfd);
bool ft_construct_listener(std::map<int, Socket *> &map_socket, Config *config, int const &epollfd);
bool ft_open_socket(struct addrinfo *info, int &socketfd);
void ft_close_socket(std::map<int, Socket *> &map_socket, int target_fd, const int &epollfd);
void ft_close_all_sockets(std::map<int, Socket *> &map_socket, const int &epollfd);

#endif
