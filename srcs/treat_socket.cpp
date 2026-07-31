/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treat_socket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 12:03:14 by ymoumene          #+#    #+#             */
/*   Updated: 2026/07/31 15:59:43 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

void ft_close_socket(std::map<int, t_socket> &map_socket, int target_fd)
{
	map_socket.erase(target_fd);
	close(target_fd);
}

bool ft_parse_request(std::map<int, t_socket> &map_socket, t_socket &target, Config *config)
{
	if (ft_parse_request(target.request_htpp, config, target.fd))
		return (true);
	if (target.request_htpp.state == COMPLETE)
	{
		struct epoll_event temp{};
		temp.data.fd = target.fd;
		temp.events = EPOLLIN, EPOLLOUT;
		if (epoll_ctl(epollfd, EPOLL_CTL_MOD, target.fd, &temp) == -1)
			return (true);
	}
	return (false);
}

bool ft_treat_socket(std::map<int, t_socket> &map_socket, struct epoll_event &event, Config *config)
{
	auto it = map_socket.find(event.data.fd);
		if (it == map_socket.end())
    		return false;
	t_socket &target = it->second;
	if (event.events & (EPOLLHUP | EPOLLHER))
	{
		ft_close_socket(map_socket, target.fd);
		return (false);
	}
	if (event.events & (EPOLLIN))
	{
		if(target.type == LISTENER)
			return (ft_create_connection(map_socket, target, config));
		if(target.type == CONNECTION)
			return (ft_parse_request(map_socket, target, config));
		if(target.type == CGI)
			return (ft_cgi(map_socket, target, config));
	}
	if (event.events & (EPOLLOUT))
	{
		if(target.type == CONNECTION)
				return (ft_send_request(map_socket, target, config));
		if(target.type == CGI)
				return (ft_cgi(map_socket, target, config));
	}
	return (false);
}
