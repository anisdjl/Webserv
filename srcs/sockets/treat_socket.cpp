/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treat_socket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 12:03:14 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/02 15:15:48 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/parsing.hpp"
#include "../includes/socket.hpp"

bool ft_parse_request(std::map<int, t_socket> &map_socket, t_socket &target, Config *config, int &epollfd)
{
	if (ft_parse_request(target.http_request, config->getServers()[target.server_index], target.fd))
		return (true);
	if (target.http_request.state == COMPLETE)
	{
		struct epoll_event temp{};
		temp.data.fd = target.fd;
		temp.events = EPOLLOUT;
		if (epoll_ctl(epollfd, EPOLL_CTL_MOD, target.fd, &temp) == -1)
			return (true);
	}
	return (false);
}

bool ft_send_request(std::map<int, t_socket> &map_socket, t_socket &target, Config *config)
{
	std::string response = ft_send(target.fd, target.http_request, config->getServers()[target.server_index], target.fd);
	unsigned int bytes_sent = 0;
	int temp_sent = 0;

	if (response.empty())
		return (true);
	while (bytes_sent < response.length())
	{
		temp_sent = send(target.fd, response.c_str() + bytes_sent, response.length() - bytes_sent, 0);
		if (temp_sent == -1)
			return (true);
		bytes_sent += temp_sent;
	}
	return (false);
}

bool ft_treat_socket(std::map<int, t_socket> &map_socket, struct epoll_event &event, Config *config, int &epollfd)
{
	auto it = map_socket.find(event.data.fd);
		if (it == map_socket.end())
    		return false;
	t_socket &target = it->second;
	
	if (event.events & (EPOLLHUP | EPOLLERR))
	{
		ft_close_socket(map_socket, target.fd);
		return (false);
	}
	if (event.events & (EPOLLIN))
	{
		if(target.type == LISTENER)
			return (ft_create_connection(map_socket, target, config));
		if(target.type == CONNECTION)
			return (ft_parse_request(map_socket, target, config, epollfd));
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
