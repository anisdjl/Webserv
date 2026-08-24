/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anis <anis@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 17:39:24 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/21 15:57:27 by anis             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../config/Config.hpp"
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
		std::time_t		_start;

		Socket() : _fd(-1), _server_index(-1), _type(LISTENER), _start(-1) { this->setStartTime();};
		Socket(int fd, int server_index, Socket_type type)
		: _fd(fd), _server_index(server_index), _type(type)  ,_start(-1)
		{this->setStartTime();};
		Socket(const Socket& src)
		: _fd(src._fd), _server_index(src._server_index), _type(src._type), _start(src._start)
		{};

	public :


		virtual ~Socket(){};
		void setStartTime(std::time_t start)
		{
			if (this->_type != LISTENER)
				this->_start = start;
		};
		void setStartTime()
		{
			if (this->_type != LISTENER)
				this->_start = std::time(NULL);
		};
		time_t getStartTime() const
		{
			return (this->_start);
		};
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
				// this->_start = src._start;
			}
			return (*this);
		};
};


bool ft_treat_socket(std::map<int, Socket *> &map_socket, struct epoll_event &event, Config *config, const int &epollfd);
bool ft_construct_listener(std::map<int, Socket *> &map_socket, Config *config, int const &epollfd);
bool ft_open_socket(struct addrinfo *info, int &socketfd);
void ft_close_socket(std::map<int, Socket *> &map_socket, int target_fd, const int &epollfd);
void ft_close_cgi(std::map<int, Socket *> &map_socket, int target_fd, const int &epollfd);
void ft_close_all_sockets(std::map<int, Socket *> &map_socket, const int &epollfd);

#endif
