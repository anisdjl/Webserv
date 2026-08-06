/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 20:03:14 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/06 20:06:37 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Socket.hpp"

class Connection : public Socket
{
    private:
        	HttpRequest		_http_request;
		    HttpResponse	_http_response;
    
    public:
        void setHttpRequest(HttpRequest& request);
		void setHttpResponse(HttpResponse& response);
        HttpRequest& getHttpRequest() const;
		HttpResponse& getHttpResponse() const;
        
        Connection();
        Connection(int fd, int server_index, Socket_type type, HttpRequest& request, HttpResponse& response);
        Connection(const Connection& other);
        ~Connection();
        Connection& operator=(const Connection& other);
};

#endif