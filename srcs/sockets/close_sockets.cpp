/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_sockets.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:23:35 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/01 18:25:44 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/socket/socket.hpp"

void ft_close_socket(std::map<int, t_socket> &map_socket, int target_fd)
{
	map_socket.erase(target_fd);
	close(target_fd);
}

void ft_close_all_sockets(std::map<int, t_socket> &map_socket)
{
	std::map<int, t_socket>::iterator it = map_socket.begin();
	
	while (it != map_socket.end())
	{
		close(it->first);
		it++;
	}
}
