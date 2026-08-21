/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anis <anis@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:27:45 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/21 18:44:33 by anis             ###   ########.fr       */
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

	std::cout << "je suis ici" << std::endl;
	int pipe_read = target.getPipeIn();
	int pipe_write = target.getPipeOut();
	// int child_fd = target.getChildFd();
	// int fd_client = target.getParentIndex();
	int epollfd = target.getEpoll();

	time_t now = std::time(NULL);

	double diff = std::difftime(now, target.getTime());
	if (diff > TIMEOUT)
	{
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

		if (it != map_socket.end() && it->second != NULL)
		{
			Connection &parent = dynamic_cast<Connection &>(*(it->second));

			parent.getHttpResponse().buildResponse(parent, config->getServer()[parent.getServerIndex()], map_socket, epollfd);

			if (parent.getHttpResponse().getState() == BUILT)
			{
				struct epoll_event temp;
				std::memset(&temp, 0, sizeof(temp));
				temp.data.fd = parent.getFd();
				temp.events = EPOLLOUT;
				epoll_ctl(epollfd, EPOLL_CTL_MOD, parent.getFd(), &temp);
			}
		}
		delete &target;
		return (true);
	}

	char buffer[BUFFER_SIZE + 1];
	int bytes_read = 0;

	Connection &parent = dynamic_cast<Connection &>(*(it->second));

	std::memset(buffer, 0, BUFFER_SIZE + 1);
	bytes_read = read(target.getPipeIn(), buffer, BUFFER_SIZE);

	if (bytes_read == -1)
		return (true);

	if (bytes_read > 0)
	{
		parent.getHttpResponse().addBody(buffer);
		return (false);
	}

	if (bytes_read == 0)
	{
		int status;
		struct epoll_event event2;
		event2.data.fd = target.getParentIndex();
		event2.events = EPOLLOUT;
		std::cout << parent.getHttpResponse().getBody();
		waitpid(target.getChildFd(), &status, WNOHANG);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			parent.getHttpRequest().setError(500);

		parent.getHttpResponse().buildResponse(parent, config->getServer()[parent.getServerIndex()], map_socket, target.getEpoll());
		if (parent.getHttpResponse().getState() == BUILT)
			epoll_ctl(target.getEpoll(), EPOLL_CTL_MOD, target.getParentIndex(), &event2);

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
	int pipe_in = target.getPipeIn();
	int pipe_out = target.getPipeOut();
	int epoll_fd = target.getEpoll();
	pid_t pid = target.getChildFd();

	int status;
	waitpid(pid, &status, WNOHANG);

	if (pipe_in != -1)
	{
		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, pipe_in, NULL);
		close(pipe_in);
		map_socket.erase(pipe_in);
	}
	if (pipe_out != -1)
	{
		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, pipe_out, NULL);
		close(pipe_out);
		map_socket.erase(pipe_out);
	}

	std::map<int, Socket *>::iterator it = map_socket.find(target.getParentIndex());
	if (it != map_socket.end() && it->second != NULL)
	{
		Connection &parent = dynamic_cast<Connection &>(*(it->second));

		parent.getHttpResponse().buildResponse(parent, config->getServer()[parent.getServerIndex()], map_socket, epoll_fd);

		if (parent.getHttpResponse().getState() == BUILT)
		{
			struct epoll_event temp;
			std::memset(&temp, 0, sizeof(temp));
			temp.data.fd = parent.getFd();
			temp.events = EPOLLOUT;
			epoll_ctl(epoll_fd, EPOLL_CTL_MOD, parent.getFd(), &temp);
		}
	}
	delete &target;
}
