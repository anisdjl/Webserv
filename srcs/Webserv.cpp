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

bool ft_webserv(Config *config)
{
	std::map <int, Socket *> map_socket;
	int epollfd = epoll_create1(0);
	struct epoll_event events[256];
	int nb_events;
	int i;

	if (epollfd == -1)
		return (true);
	if(ft_construct_listener(map_socket, config, epollfd))
		return (ft_close_all_sockets(map_socket, epollfd), true);
	signal(SIGINT, ft_handler);
	while(run)
	{
		nb_events = epoll_wait(epollfd, events, 256 , 5000);
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
	}
	ft_close_all_sockets(map_socket, epollfd);
	return (false);
}
