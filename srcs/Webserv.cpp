/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjelili <adjelili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 23:23:13 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/05 14:25:58 by adjelili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"
#include "../includes/config/Config.hpp"
#include "../includes/socket/Socket.hpp"

static volatile sig_atomic_t run = 1;

void ft_handler(int signal)
{
	(void) signal;
	run = 0;
}

bool ft_open_socket(struct addrinfo *info, int &socketfd)
{
	struct addrinfo *temp;
	int flags_fcntl;
	
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
	flags_fcntl = fcntl(socketfd, F_GETFL);
	if (flags_fcntl == -1 || fcntl(socketfd, F_SETFL, flags_fcntl | O_NONBLOCK) == -1)
		return (close(socketfd), true);
	return(false);
}

bool ft_webserv(Config *config)
{
	std::map <int, Socket> map_socket;
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
			if(ft_treat_socket(map_socket, events[i], config, epollfd))
				return (true);
			i++;
		}
	}
	ft_close_all_sockets(map_socket);
	return (false);
}
