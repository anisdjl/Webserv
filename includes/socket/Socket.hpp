/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 17:39:24 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 15:39:00 by ymoumene         ###   ########.fr       */
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
		
		Socket();
		Socket(int fd, int server_index, Socket_type type);
		Socket(const Socket& src);
		
	public :

		
		~Socket();
		int &getFd();
		int getFd() const;
		int getServerIndex() const;
		Socket_type getType() const;
		void setFd(int fd);
		void setServerIndex(int index);
		void setType(Socket_type type);
		Socket& operator=(const Socket& src);
};

bool ft_treat_socket(std::map<int, Socket> &map_socket, struct epoll_event &event, Config *config, const int &epollfd);
bool ft_construct_listener(std::map <int, Socket> &map_socket, Config *config, int const &epollfd);
bool ft_open_socket(struct addrinfo *info, int &socketfd);
void ft_close_socket(std::map<int, Socket> &map_socket, int target_fd, const int &epollfd);
void ft_close_all_sockets(std::map<int, Socket> &map_socket, const int &epollfd);

#endif
