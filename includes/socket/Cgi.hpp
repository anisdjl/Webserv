/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 20:02:41 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 16:50:11 by ymoumene         ###   ########.fr       */
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
        int getParentIndex() const
		{
			return (this->_parent_index);
		};
        void setParentIndex(int index)
		{
			this->_parent_index = index;
		};
        Cgi() : Socket(), _parent_index(-1)
		{
			this->_type = CGI;
		};
        Cgi(int fd, int server_index, int parent_index) 
		: Socket(fd, server_index, CGI), _parent_index(parent_index)
		{};
		Cgi(const Cgi& src) : Socket(src), _parent_index(src._parent_index)
		{
			
		};
        ~Cgi()
		{

		};
        Cgi& operator=(const Cgi& src)
		{
			if (this != &src)
			{
				this->_fd = src._fd;
				this->_server_index = src._server_index;
				this->_type = src._type;
				this->_parent_index = src._parent_index;
			}
			return (*this);
		};
};

#endif