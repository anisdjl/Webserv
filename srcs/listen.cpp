/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 10:50:42 by ymoumene          #+#    #+#             */
/*   Updated: 2026/07/28 17:59:08 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/parsing.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


bool ft_open_socket(struct addrinfo *info, int &socketfd)
{
	struct addrinfo *temp;
	
	temp = info;
	while(temp != NULL)
	{
		socketfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		if (socketfd == -1)
			continue ;
		if (bind(socketfd, info->ai_addr, info->ai_addrlen))
			break ;
		close(socketfd);
		temp = temp->ai_next;		
	}
	freeaddrinfo(info);
	if (!temp)
		return(true)
	return(false);
}

bool ft_listener(std::string &listener, int &socketfd)
{
	struct addrinfo *info;
	struct addrinfo hints{};
1
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
	listen(socketfd, SOMAXCONN);
	return(false);
}
