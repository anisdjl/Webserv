/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 17:39:24 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/04 18:11:48 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "../webserv.hpp"
#include "../config/Config.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"

enum Socket_type
{
    LISTENER,
    CONNECTION,
    CGI
};


class Socket
{
	private:
		int         	_fd;
    	int         	_server_index;
		int 	   		_parent_index;
    	Socket_type 	_type;
		HttpRequest		_http_request;
		HttpResponse	_http_response;

	public :

	Socket();
	Socket(int fd, int server_index, Socket_type type, HttpRequest& request, HttpResponse& response);
	Socket(const Socket& other);
	~Socket();
	int &getFd();
	int getFd() const;
	int getServerIndex() const;
	int getParentIndex() const;
	Socket_type getType() const;
	HttpRequest& getHttpRequest() const;
	HttpResponse& getHttpResponse() const;
	
	void setFd(int fd);
	void setServerIndex(int index);
	void setType(Socket_type type);
	void setHttpRequest(HttpRequest& request);
	void setHttpResponse(HttpResponse& response);
	void setParentIndex(int index);
	Socket& operator=(const Socket& other);
};

bool ft_treat_socket(std::map<int, Socket> &map_socket, struct epoll_event &event, Config *config, int &epollfd);
bool ft_construct_listener(std::map <int, Socket> &map_socket, Config *config, int const &epollfd);
bool ft_open_socket(struct addrinfo *info, int &socketfd);
void ft_close_socket(std::map<int, Socket> &map_socket, int target_fd);
void ft_close_all_sockets(std::map<int, Socket> &map_socket);

#endif
