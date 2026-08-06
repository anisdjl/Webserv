/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CloseSockets.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:23:35 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/06 19:15:22 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/socket/Socket.hpp"

void ft_close_socket(std::map<int, Socket> &map_socket, int target_fd, const int &epollfd)
{
	epoll_ctl(epollfd, EPOLL_CTL_DEL, target_fd, NULL);
	map_socket.erase(target_fd);
	close(target_fd);
}

void ft_close_all_sockets(std::map<int, Socket> &map_socket, const int &epollfd)
{	
	while (map_socket.size() > 0)
		ft_close_socket(map_socket, map_socket.begin()->first, epollfd);
	close(epollfd);
}
