/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 17:36:31 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/05 19:37:43 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/socket/Socket.hpp"

bool ft_listener(std::string &listener, int &socketfd)
{
	struct addrinfo *info;
	struct addrinfo hints;

	std::memset(&hints, 0, sizeof(hints));
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

bool ft_construct_listener(std::map <int, Socket> &map_socket, Config *config, int const &epollfd)
{
	Socket temp_socket;
	struct epoll_event temp;
	int i = 0; 

	std::memset(&temp, 0, sizeof(temp));
	temp_socket.setType(LISTENER);
	while(i < config->getServer().size())
	{
		if(ft_listener(config->getServer()[i].getListen(), temp_socket.getFd()))
			return (true);
		temp_socket.setServerIndex(i);
		map_socket.insert(std::make_pair(temp_socket.getFd(), temp_socket));
		temp.data.fd = temp_socket.getFd();
		temp.events = EPOLLIN;
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, temp_socket.getFd(), &temp) == -1)
			return (true);
		i++;
	}
	return (false);
}
