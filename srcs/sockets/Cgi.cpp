/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjelili <adjelili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:27:45 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/19 15:06:08 by adjelili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"
#include "../includes/socket/Socket.hpp"
#include "../includes/socket/Cgi.hpp"
#include "../includes/socket/Connection.hpp"

bool ft_cgi_in(std::map<int, Socket*> &map_socket, Cgi &target, Config *config)
{
	char	buffer[BUFFER_SIZE + 1];
	int	bytes_read = 0 ;
	
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
		// utiliser le parent pour remplir la reponse
		
		return (false);
	}
	
	if (bytes_read == 0) // c'est qu'on a recu toute la reponse
	{
		int	status;

		waitpid(target.getFd(), &status, WNOHANG);
		epoll_ctl(target.getEpoll(), EPOLL_CTL_DEL, target.getPipeIn(), NULL);
		epoll_ctl(target.getEpoll(), EPOLL_CTL_DEL, target.getPipeOut(), NULL);
		
		
		// appeler build response
		struct epoll_event event;
		event.data.fd = target.getParentIndex();
		event.events = EPOLLOUT;
		
		epoll_ctl(target.getEpoll(), EPOLL_CTL_MOD, target.getParentIndex(), &event);
		// sendresponse(); // renvoyer la reponse demander a evan
		// remettre en epollout
	}
	return (false);
}

bool ft_cgi_out(std::map<int, Socket> &map_socket, Socket &target, Config *config)
{
	// je recupe le body de la requete,
	// je le mets dans un buffer
	// j'ecris ce buffer dans le pipe_out[1]
	// pour le moment c'est le seul trcu qui me vient a l'esprit
	
	return (false);
}

// bool ft_cgi_hup(std::map<int, Socket> &map_socket, Socket &target, Config *config, const int &epollfd) // ici c'est quand il y a unb soucis on doit tout liberer et fermer
// {
// 	// close le socket du cgi
// 	Socket &parent = map_socket.find(target.getParentIndex())->second;
// 	ft_close_socket(map_socket, target.getFd(), epollfd);
// 	parent.getHttpResponse().buildResponse(parent.getHttpRequest(), config->getServer()[parent.getServerIndex()]);
// 	if (parent.getHttpResponse().getState() == BUILT)
// 	{	
// 		struct epoll_event temp;
// 		std::memset(&temp, 0, sizeof(temp));
// 		temp.data.fd = parent.getFd();
// 		temp.events  = EPOLLOUT;
// 		if (epoll_ctl(epollfd, EPOLL_CTL_MOD, parent.getFd(), &temp) == -1)
// 			return (true);
// 	}
// }
