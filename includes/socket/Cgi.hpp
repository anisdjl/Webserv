/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 20:02:41 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/06 20:07:24 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "Socket.hpp"

class Cgi : public Socket
{
    private :
            int 	   		_parent_index;
    public:
        int getParentIndex() const;
        void setParentIndex(int index);
        Cgi();
        Cgi(int fd, int server_index, Socket_type type, HttpRequest& request, HttpResponse& response);
        Cgi(const Cgi& other);
        ~Cgi();
        Cgi& operator=(const Cgi& other);
};

#endif