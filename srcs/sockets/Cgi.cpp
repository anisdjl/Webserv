/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjelili <adjelili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 15:27:45 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/18 18:29:51 by adjelili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"
#include "../includes/socket/Socket.hpp"


bool ft_cgi_in(std::map<int, Socket> &map_socket, Socket &target, Config *config) // c'est que le script est entrain de renvoyer des infos dans le pipe
{
	// lire le resultat
	char	buffer[BUFFER_SIZE + 1];
	int	bytes_read = 0 ;
	
	std::memset(buffer, 0, BUFFER_SIZE +1);
	bytes_read = read(pipe_fd, buffer, BUFFER_SIZE);
	if (bytes_read == -1)
		return (true);

	if (bytes_read > 0)
	{
		// on rajoute
		return (false);
	}
	
	if (bytes_read == 0) // c'est qu'on a recu toute la reponse
	{
		int	status;

		waitpid(fd_process, &status, WNOHANG);
		epoll_ctl(epollfd, EPOLL_CTL_DEL, )
		sendresponse();
		// remettre ne epollout
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
