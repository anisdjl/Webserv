/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjelili <adjelili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:27:45 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/24 15:30:29 by adjelili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Webserv.hpp"
#include "../../includes/socket/Socket.hpp"
#include "../../includes/socket/Cgi.hpp"
#include "../../includes/socket/Connection.hpp"

bool	ft_cgi_in(std::map<int, Socket *> &map_socket, Cgi &target, Config *config)
{
	std::map<int, Socket *>::iterator it = map_socket.find(target.getParentIndex());
	if (it == map_socket.end())
		return (true);
	(void)config;

	char buffer[BUFFER_SIZE + 1];
	int bytes_read = 0;

	Connection &parent = dynamic_cast<Connection &>(*(it->second));

	std::memset(buffer, 0, BUFFER_SIZE + 1);
	bytes_read = read(target.getPipeIn(), buffer, BUFFER_SIZE);

	if (bytes_read == -1)
		return (true);

	if (bytes_read > 0)
	{
		// std::cout << buffer << std::endl;
		parent.getHttpResponse().addBody(buffer);
		// std::cout << "nb bytes read " << bytes_read << std::endl;
		return (false);
	}
	return (false);
}

bool	ft_cgi_out(std::map<int, Socket *> &map_socket, Cgi &target, Config *config)
{
	(void)map_socket;
	(void)target, (void)config;

	// ecrire petit a petit et suivre ce qui a ete ecris ou pas encore
	// une fois tout ecrit, on ferme direct pour envoyer le signal EOF au script

	std::cout << "ca marche pas ici" << std::endl;
	// epoll_ctl(target.getEpoll(), EPOLL_CTL_DEL, target.getPipeIn(), NULL);
	// close(target.getPipeIn());
	// map_socket.erase(target.getPipeIn());
	// // on supprime le pipe de epoll
	// // on le retire de map socket mais techniquement je ne peux pas pcq je l'enregistre avec
	return (false);
}

void	ft_cgi_hup(std::map<int, Socket *> &map_socket, Cgi &target, Config *config)
{
	// rajouter le waitpid pour checker si l'execve n'a pas foire

	int status;
	struct epoll_event event2;
	event2.data.fd = target.getParentIndex();
	event2.events = EPOLLOUT;
	// std::cout << "je suis pas dans bytes == 0" << std::endl;
	std::map<int, Socket *>::iterator it = map_socket.find(target.getParentIndex());
	if (it == map_socket.end())
		return ;
	Connection &parent = dynamic_cast<Connection &>(*(it->second));
	
	std::ostringstream oss;
	oss << parent.getHttpResponse().getBody().size();
	parent.getHttpResponse().setHeader("Server", "WeebServ");
	parent.getHttpResponse().setHeader("Content-Type", parent.getHttpResponse()._findContentType(target.getReqPath()));
	parent.getHttpResponse().setHeader("Content-Length", oss.str());
	parent.getHttpResponse().setHeader("Connection", "keep-alive");
	std::cout << checkContentTypeScript(parent) << std::endl;

	// std::cout << parent.getHttpResponse().getBody();

	waitpid(target.getChildFd(), &status, WNOHANG);
	// if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	// 	parent.getHttpRequest().setError(500);

	// std::cout << "je suis pas dans bytes == 0" << std::endl;
	parent.getHttpResponse().buildResponse(parent, config->getServer()[parent.getServerIndex()], map_socket, target.getEpoll());
	if (parent.getHttpResponse().getState() == BUILT)
		epoll_ctl(target.getEpoll(), EPOLL_CTL_MOD, target.getParentIndex(), &event2);

		// ft_cgi_close(target, map_socket);
	int pipe_read = target.getPipeIn();
	int pipe_write = target.getPipeOut();
	int epollfd = target.getEpoll();
	if (pipe_read != -1)
	{
		epoll_ctl(epollfd, EPOLL_CTL_DEL, pipe_read, NULL);
		close(pipe_read);
		map_socket.erase(pipe_read);
	}
	if (pipe_write != -1)
	{
		epoll_ctl(epollfd, EPOLL_CTL_DEL, pipe_write, NULL);
		close(pipe_write);
		map_socket.erase(pipe_write);
	}
	delete &target;
	//	delete &target;
}


void	ft_cgi_close(Cgi &target, std::map<int, Socket *> &map_socket)
{
	int pipe_read = target.getPipeIn();
	int pipe_write = target.getPipeOut();
	int epollfd = target.getEpoll();
	if (pipe_read != -1)
	{
		epoll_ctl(epollfd, EPOLL_CTL_DEL, pipe_read, NULL);
		close(pipe_read);
		map_socket.erase(pipe_read);
	}
	if (pipe_write != -1)
	{
		epoll_ctl(epollfd, EPOLL_CTL_DEL, pipe_write, NULL);
		close(pipe_write);
		map_socket.erase(pipe_write);
	}
	delete &target;
}

std::string	checkContentTypeScript(Connection &client)
{
	std::string	body = client.getHttpResponse().getBody();
	std::string	contentType = "hello";
	
	
	std::cout << body << std::endl;
	return (contentType);
}