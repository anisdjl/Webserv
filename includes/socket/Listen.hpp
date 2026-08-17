/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 20:03:12 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/17 16:23:14 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_HPP
#define LISTEN_HPP

#include "Socket.hpp"

class Listen : public Socket
{
    public:
        Listen() : Socket(){};
        Listen(int fd, int server_index): Socket(fd, server_index, LISTENER)
		{};
        Listen(const Listen& src):
		Socket(src){};
        ~Listen(){};
        Listen& operator=(const Listen& src	) 
		{
			if (this != &src)
			{
				this->_fd = src._fd;
				this->_server_index = src._server_index;
				this->_type = src._type;
			}
			return (*this);
		};
};
#endif