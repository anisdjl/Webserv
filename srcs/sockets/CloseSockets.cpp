/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CloseSockets.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:23:35 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/05 10:35:34 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/socket/Socket.hpp"

void ft_close_socket(std::map<int, Socket> &map_socket, int target_fd)
{
	map_socket.erase(target_fd);
	close(target_fd);
}

void ft_close_all_sockets(std::map<int, Socket> &map_socket)
{
	std::map<int, Socket>::iterator it = map_socket.begin();
	
	while (it != map_socket.end())
	{
		close(it->first);
		it++;
	}
}
