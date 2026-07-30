/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:23:13 by ymoumene          #+#    #+#             */
/*   Updated: 2026/07/30 23:29:18 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/connection.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


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

bool ft_construct_listener(std::vector<t_socket> &vec_socket, Config *config, int const &epollfd)
{
	t_socket temp_socket;
	struct epoll_event temp{};
	int i = 0; 

	temp_socket.type = LISTENER;
	while(i < config->listen.size())
	{
		if(ft_listener(config->listen[i], temp_socket.fd))
			return (true);
		vec_socket.push_back(temp_socket.fd);
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
	std::vector<t_socket> vec_socket;
	int epollfd = epoll_create(1);

	if (epollfd == -1 || ft_construct_listener(vec_socket, config, epollfd))
		return (true);
	return (false);
}
