/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:27:45 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 15:46:15 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/socket/socket.hpp"


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

bool ft_cgi_hup(std::map<int, Socket> &map_socket,Socket &target, Config *config, const int &epollfd)
{
	// close le socket du cgi
	Socket &parent = map_socket.find(target.getParentIndex())->second;
	ft_close_socket(map_socket, target.getFd(), epollfd);
	parent.getHttpResponse().buildResponse(parent.getHttpRequest(), config->getServer()[parent.getServerIndex()]);
	if (parent.getHttpResponse().getState() == BUILT)
	{	
		struct epoll_event temp;
		std::memset(&temp, 0, sizeof(temp));
		temp.data.fd = parent.getFd();
		temp.events  = EPOLLOUT;
		if (epoll_ctl(epollfd, EPOLL_CTL_MOD, parent.getFd(), &temp) == -1)
			return (true);
	}
}
