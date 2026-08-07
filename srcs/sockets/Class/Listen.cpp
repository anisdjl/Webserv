/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 14:51:32 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 15:33:58 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket"

#include "../includes/socket/Listen.hpp"

Listen::Listen() : Socket()
{
	this->_type = LISTENER;
}

Listen::Listen(int fd, int server_index) : Socket(fd, server_index, LISTENER)
{}

Listen::Listen(const Listen& src) : Socket(src)
{}

Listen::~Listen() {}