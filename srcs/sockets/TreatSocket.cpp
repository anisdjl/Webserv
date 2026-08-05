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

#include "../../includes/socket/Socket.hpp"

bool ft_cgi_in(std::map<int, Socket> &map_socket,Socket &target, Config *config)
{
	int bytes_read = 0;
	Socket &parent = map_socket.find(target.getParentIndex())->second;
	char buffer[BUFFER_SIZE + 1];

	std::memset(buffer, 0, BUFFER_SIZE + 1);
	bytes_read = recv(target.getFd(), buffer, BUFFER_SIZE, 0);
	if (bytes_read == -1)
		return (true);
	parent.getHttpResponse().setBody(parent.getHttpResponse().getBody() + std::string(buffer, bytes_read));
	return (false);
}

bool ft_cgi_out(std::map<int, Socket> &map_socket,Socket &target, Config *config)
{

}

bool ft_cgi_hup(std::map<int, Socket> &map_socket,Socket &target, Config *config, int &epollfd)
{
	// close le socket du cgi

	target.getHttpResponse().buildResponse(target.getHttpRequest(), config->getServer()[target.getServerIndex()]);
	if (target.getHttpResponse().getState() == BUILT)
	{	
		struct epoll_event temp;
		std::memset(&temp, 0, sizeof(temp));
		temp.data.fd = target.getFd();
		temp.events  = EPOLLOUT;
		if (epoll_ctl(epollfd, EPOLL_CTL_MOD, target.getFd(), &temp) == -1)
			return (true);
	}
}

bool ft_parse_request(std::map<int, Socket> &map_socket, Socket &target, Config *config, int &epollfd)
{
	int bytes_read = 0;
	char buffer[BUFFER_SIZE + 1];


	if (target.getHttpRequest().getState() == INCOMPLETE)
	{	
		std::memset(buffer, 0, BUFFER_SIZE + 1);
		bytes_read = recv(target.getFd(), buffer, BUFFER_SIZE, 0);
		if (bytes_read == -1)
			return (true);
		if (ft_parse_http_request(target.getHttpRequest(), config->getServer()[target.getServerIndex()], buffer, bytes_read))
			return (true);
	}
	if (target.getHttpRequest().getState() == COMPLETE)
	{
		if(target.getHttpResponse().getState() == NOT_BUILT)
		{	
			target.getHttpResponse().buildResponse(target.getHttpRequest(), config->getServer()[target.getServerIndex()]);
		}
		if (target.getHttpResponse().getState() == BUILT)
		{	
			struct epoll_event temp;
			std::memset(&temp, 0, sizeof(temp));
			temp.data.fd = target.getFd();
			temp.events  = EPOLLOUT;
			if (epoll_ctl(epollfd, EPOLL_CTL_MOD, target.getFd(), &temp) == -1)
				return (true);
		}
	}
	return (false);
}

bool ft_send_request(std::map<int, Socket> &map_socket, Socket &target, Config *config, int &epollfd)
{
	unsigned int bytes_sent = 0;
	int temp_sent = 0;
	struct epoll_event temp;
	std ::string response = target.getHttpResponse().getResponse();

	if (response.empty())
		return (true);
	while (bytes_sent < response.length())
	{
		temp_sent = send(target.getFd(), response.c_str() + bytes_sent, response.length() - bytes_sent, 0);
		if (temp_sent == -1)
			return (true);
		bytes_sent += temp_sent;
	}
	std::memset(&temp, 0, sizeof(temp));
	temp.data.fd = target.getFd();
	temp.events = EPOLLIN;
	target.getHttpRequest().resetRequest();
	target.getHttpResponse().resetResponse();
	std::memset(&temp, 0, sizeof(temp));
	if (epoll_ctl(epollfd, EPOLL_CTL_MOD, target.getFd(), &temp) == -1)
		return (true);
	return (false);
}

bool ft_create_connection(std::map<int, Socket> &map_socket, Socket &target , int &epollfd)
{
	Socket temp;
	struct epoll_event temp_event;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	int flags_fcntl;
	
	std::memset(&their_addr, 0, sizeof(their_addr));
	addr_size = sizeof (their_addr);
	temp = target;
	temp.setType(CONNECTION);
	temp.setFd(accept(target.getFd(), (struct sockaddr *)&their_addr, &addr_size));
	if (temp.getFd() == -1)
		return (true);
	flags_fcntl = fcntl(temp.getFd(), F_GETFL);
	if (flags_fcntl == -1 || fcntl(temp.getFd(), F_SETFL, flags_fcntl | O_NONBLOCK) == -1)
		return (close(temp.getFd()), true);
	temp_event.data.fd = temp.getFd();
	temp_event.events = EPOLLIN;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, temp.getFd(), &temp_event) == -1)
			return (close(temp.getFd()), true);
	map_socket.insert(std::make_pair(temp.getFd(), temp));
	return (false);
}

bool ft_treat_socket(std::map<int, Socket> &map_socket, struct epoll_event &event, Config *config, int &epollfd)
{
	std::map<int, Socket>::iterator it = map_socket.find(event.data.fd);
		if (it == map_socket.end())
    		return false;
	Socket &target = it->second;
	
	if (event.events & (EPOLLHUP | EPOLLERR))
	{
		if (target.getType() == CGI)
			ft_cgi_hup(map_socket, target, config, epollfd);
		else
			ft_close_socket(map_socket, target.getFd());
		return (false);
	}
	if (event.events & (EPOLLIN))
	{
		if(target.getType() == LISTENER)
			return (ft_create_connection(map_socket, target, epollfd));
		if(target.getType() == CONNECTION)
			return (ft_parse_request(map_socket, target, config, epollfd));
		if(target.getType() == CGI)
			return (ft_cgi_in(map_socket, target, config));
		}
	if (event.events & (EPOLLOUT))
	{
		if(target.getType() == CONNECTION)
				return (ft_send_request(map_socket, target, config, epollfd));
		if(target.getType() == CGI)
				return (ft_cgi_out(map_socket, target, config));
	}
	return (false);
}
