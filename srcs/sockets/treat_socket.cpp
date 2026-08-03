 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treat_socket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 12:03:14 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/03 10:03:12 by ymoumene         ###   ########.fr       */
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
	
	std::string response = buildresponse(target.http_request, config->getServers()[target.server_index]);
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
	struct epoll_event temp{};
	temp.data.fd = target.fd;
	temp.events = EPOLLIN;
	// maybe reset la request
	if (epoll_ctl(epollfd, EPOLL_CTL_MOD, target.fd, &temp) == -1)
		return (true);
	return (false);
}

bool ft_create_connection(std::map<int, t_socket> &map_socket, t_socket &target)
{
	t_socket temp;
	struct epoll_event temp_event{};
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	int flags_fcntl;
	
	addr_size = sizeof (their_addr);
	temp.fd = accept(target.fd, (struct sockaddr *)&their_addr, &addr_size);
	if (temp.fd == -1)
		return (true);
	flags_fcntl = fcntl(temp.fd, F_GETFL);
	if (flags_fcntl == -1 || fcntl(temp.fd, F_SETFL, flags_fcntl | O_NONBLOCK) == -1)
		return (close(temp.fd), true);
	temp.server_index = target.server_index;
	temp.type = CONNECTION;
	temp_event.data.fd = temp_socket.fd;
	temp_event.events = EPOLLIN;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, temp.fd, &temp_event) == -1)
			return (close(temp.fd), true);
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
			return (ft_create_connection(map_socket, target));
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
