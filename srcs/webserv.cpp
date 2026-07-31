/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:23:13 by ymoumene          #+#    #+#             */
/*   Updated: 2026/07/31 14:08:59 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>


volatile sig_atomic_t run = 1;


void ft_handler(int signal)
{
	(void) signal;
	run = 0;
}

bool ft_open_socket(struct addrinfo *info, int &socketfd)
{
	struct addrinfo *temp;
	
	temp = info;
	while(temp)
	{
		socketfd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
		if (socketfd == -1)
		{
			temp = temp->ai_next;
			continue ;
		}
		if (bind(socketfd, temp->ai_addr, temp->ai_addrlen) == 0)
			break ;
		close(socketfd);
		temp = temp->ai_next;		
	}
	freeaddrinfo(info);
	if (!temp)
		return(true);
	return(false);
}

bool ft_listener(std::string &listener, int &socketfd)
{
	struct addrinfo *info;
	struct addrinfo hints{};

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	if (getaddrinfo(NULL,  listener.c_str(), &hints, &info))
	{
		std::cerr << "Error while opening socket listener." <<  std::endl;
		return (true);
	}
	if (ft_open_socket(info, socketfd))
	{
		std::cerr << "Error while opening sockets listener." <<  std::endl;
		return (true);
	}
	if (listen(socketfd, SOMAXCONN) == -1)
	{
		close(socketfd);
		std::cerr << "Error while listening sockets listener." <<  std::endl;
		return (true);
	}
	return(false);
}

bool ft_construct_listener(std::map <int, t_socket> &map_socket, Config *config, int const &epollfd)
{
	t_socket temp_socket;
	struct epoll_event temp{};
	int i = 0; 

	temp_socket.type = LISTENER;
	while(i < config->listen.size())
	{
		if(ft_listener(config->listen[i], temp_socket.fd))
			return (true);
		map_socket.insert(std::make_pair(temp_socket.fd, temp_socket));
		temp.data.fd = temp_socket.fd;
		temp.events = EPOLLIN;
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, temp_socket.fd, &temp) == -1)
			return (true);
		i++;
	}
	return (false);
}

bool ft_webserv(Config *config)
{
	std::map <int, t_socket> map_socket;
	int epollfd = epoll_create1(0);
	struct epoll_event events[128];
	int nb_events;
	int i;

	if (epollfd == -1 || ft_construct_listener(map_socket, config, epollfd))
		return (true);
	signal(SIGINT, ft_handler);
	while(run)
	{
		nb_events = epoll_wait(epollfd, events, 128 , -1);
		if (nb_events == 0)
			continue ;
		if (nb_events == -1)
			return (true); //erreur a traiter
		i = 0;
		while (i < nb_events)
		{
			if(ft_treat_socket(map_socket, events[i], config))
				return (true);
			i++;
		}
	}
	return (false);
}
