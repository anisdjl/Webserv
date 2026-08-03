/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 17:39:24 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/03 16:29:04 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SOCKET_HPP
#define SOCKET_HPP

#define BUFFER_SIZE 8192
#include "webserv.hpp"
#include "parsing.hpp"

enum Socket_type
{
    LISTENER,
    CONNECTION,
    CGI
};


Class Socket
{
	private:
		int         	_fd;
    	int         	_server_index;
		int 	   		_parent_index;
    	Socket_type 	_type;
		Request     	&_http_request;
		HttpResponse	&_http_response;

	public :

	Socket();
	Socket(int fd, int server_index, Socket_type type, Request& request);
	Socket(const Socket& other);
	~Socket();
	int &getFd();
	const int getFd() const;
	int getServerIndex() const;
	int getParentIndex() const;
	Socket_type getType() const;
	Request& getHttpRequest() const;
	void setFd(int fd);
	void setServerIndex(int index);
	void setType(Socket_type type);
	void setHttpRequest(Request& request);
	void setParentIndex(int index);
	Socket& operator=(const Socket& other);
};

bool ft_treat_socket(std::map<int, Socket> &map_socket, struct epoll_event &event, Config *config, int &epollfd);
bool ft_construct_listener(std::map <int, Socket> &map_socket, Config *config, int const &epollfd);
bool ft_open_socket(struct addrinfo *info, int &socketfd);
void ft_close_socket(std::map<int, Socket> &map_socket, int target_fd);
void ft_close_all_sockets(std::map<int, Socket> &map_socket);

#endif