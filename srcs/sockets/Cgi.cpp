/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjelili <adjelili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:27:45 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/20 16:07:05 by adjelili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Webserv.hpp"
#include "../../includes/socket/Socket.hpp"
#include "../../includes/socket/Cgi.hpp"
#include "../../includes/socket/Connection.hpp"

bool ft_cgi_in(std::map<int, Socket*> &map_socket, Cgi &target, Config *config)
{
	time_t	actual_time = std::time(nullptr);
	
	
	double diff = std::difftime(actual_time, target.getTime());

	char	buffer[BUFFER_SIZE + 1];
	int		bytes_read = 0 ;

	std::map<int, Socket *>::iterator it = map_socket.find(target.getParentIndex());
	if (it == map_socket.end())
		return (true);
	Connection &parent = dynamic_cast<Connection &>(*(it->second));

	std::memset(buffer, 0, BUFFER_SIZE +1);
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
		int	status;
		struct epoll_event event2;
		event2.data.fd = target.getParentIndex();
		event2.events = EPOLLOUT;

		waitpid(target.getFd(), &status, WNOHANG);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			parent.getHttpRequest().setError(500);

		parent.getHttpResponse().buildResponse(parent, config->getServer()[parent.getServerIndex()], map_socket, target.getEpoll());
		if (parent.getHttpResponse().getState() == BUILT)
			epoll_ctl(target.getEpoll(), EPOLL_CTL_MOD, target.getParentIndex(), &event2);

		epoll_ctl(target.getEpoll(), EPOLL_CTL_DEL, target.getPipeIn(), NULL);
		epoll_ctl(target.getEpoll(), EPOLL_CTL_DEL, target.getPipeOut(), NULL);
	
		std::cout << parent.getHttpResponse().getResponse() << std::endl;

		delete [] &target;
		// delete le cgi de la heap
		map_socket.erase(target.getPipeIn());
		map_socket.erase(target.getPipeOut());
	}
	return (false);
}

bool ft_cgi_out(std::map<int, Socket> &map_socket, Cgi &target, Config *config)
{
	(void)map_socket; (void)target, (void)config;
	
	
	// ecrire petit a petit et suivre ce qui a ete ecris ou pas encore
	// une fois tout ecrit, on ferme direct pour envoyer le signal EOF au script
	// on supprime le pipe de epoll
	// on le retire de map socket mais techniquement je ne peux pas pcq je l'enregistre avec 
	return (false);
}

void	ft_cgi_hup(std::map<int, Socket> &map_socket, Cgi &target, Config *config) // ici c'est quand il y a unb soucis on doit tout liberer et fermer
{
	(void)map_socket; (void)target, (void)config;
	// close le socket du cgi
	// Socket &parent = map_socket.find(target.getParentIndex())->second;
	// ft_close_socket(map_socket, target.getFd(), epollfd);
	// parent.getHttpResponse().buildResponse(parent.getHttpRequest(), config->getServer()[parent.getServerIndex()]);
	// if (parent.getHttpResponse().getState() == BUILT)
	// {	
	// 	struct epoll_event temp;
	// 	std::memset(&temp, 0, sizeof(temp));
	// 	temp.data.fd = parent.getFd();
	// 	temp.events  = EPOLLOUT;
	// 	if (epoll_ctl(epollfd, EPOLL_CTL_MOD, parent.getFd(), &temp) == -1)
	// 		return (true);
	// }
}
