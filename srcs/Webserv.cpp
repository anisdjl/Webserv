#include "../includes/Webserv.hpp"
#include "../includes/config/Config.hpp"
#include "../includes/socket/Socket.hpp"
#include "../includes/socket/Cgi.hpp"

static volatile sig_atomic_t run = 1;

void ft_handler(int signal)
{
	(void) signal;
	run = 0;
}

void ft_timeout_sockets(std::map<int, Socket *> &map_socket,const int &epollfd, Config *config)
{
	std::time_t end = std::time(NULL);

	std::vector<int> fd_to_destroy;
	
	for (std::map<int, Socket *>::iterator it = map_socket.begin(); it != map_socket.end(); ++it)
	{
		
		double elapsed = std::difftime(end, it->second->getStartTime());
		if (it->second->getStartTime() != -1 && elapsed > TIMEOUT)
			fd_to_destroy.push_back(it->first);
	}
	for (size_t i = 0; i < fd_to_destroy.size(); ++i)
	{
		Socket *socket = map_socket.find(fd_to_destroy[i])->second;
		if (socket->getType() == CONNECTION)
			ft_close_socket(map_socket, socket->getFd(), epollfd);
		else if (socket->getType() == CGI)
			ft_cgi_hup(map_socket, dynamic_cast<Cgi &>(*socket), config);
	}
}

bool ft_webserv(Config *config)
{
	std::map <int, Socket *> map_socket;
	int epollfd = epoll_create1(0);
	struct epoll_event events[128];
	int nb_events;
	int i;

	if (epollfd == -1)
		return (true);
	if(ft_construct_listener(map_socket, config, epollfd))
		return (ft_close_all_sockets(map_socket, epollfd), true);
	signal(SIGINT, ft_handler);
	while(run)
	{
		nb_events = epoll_wait(epollfd, events, 128 , 5000);
		if (nb_events == -1)
		{
			if (errno == EINTR)
				break ;
			else
				return (ft_close_all_sockets(map_socket, epollfd), true);
		}
		i = 0;
		while (i < nb_events)
		{
			if(ft_treat_socket(map_socket, events[i], config, epollfd))
				return (ft_close_all_sockets(map_socket, epollfd), true);
			i++;
		}
		ft_timeout_sockets(map_socket, epollfd, config);
	}
	ft_close_all_sockets(map_socket, epollfd);
	return (false);
}
