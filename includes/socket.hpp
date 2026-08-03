/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 17:39:24 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/03 15:14:29 by ymoumene         ###   ########.fr       */
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


typedef struct s_socket
{
    int         fd;
    int         server_index;
    Socket_type type;
	Request     http_request;
} t_socket;

bool ft_treat_socket(std::map<int, t_socket> &map_socket, struct epoll_event &event, Config *config, int &epollfd);
bool ft_construct_listener(std::map <int, t_socket> &map_socket, Config *config, int const &epollfd);
bool ft_open_socket(struct addrinfo *info, int &socketfd);
void ft_close_socket(std::map<int, t_socket> &map_socket, int target_fd);
void ft_close_all_sockets(std::map<int, t_socket> &map_socket);

#endif