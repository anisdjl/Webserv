/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:11:10 by ymoumene          #+#    #+#             */
/*   Updated: 2026/07/30 23:07:17 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  CONNECTION_HPP
#define CONNECTION_HPP

enum Socket_type
{
    LISTENER,
    CONNECTION,
    CGI
};


typedef struct s_socket
{
    int fd;
    Socket_type type;
} t_socket;

#endif