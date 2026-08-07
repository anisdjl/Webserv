/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 20:03:12 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 14:52:52 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_HPP
#define LISTEN_HPP

#include "Socket.hpp"

class Listen : public Socket
{
    public:
        Listen();
        Listen(int fd, int server_index);
        Listen(const Listen& other);
        ~Listen();
        Listen& operator=(const Listen& other);
};

#endif