#include "../../includes/socket/Socket.hpp"
#include "../../includes/socket/Listen.hpp"

bool ft_open_socket_listener(struct addrinfo *info, int &socketfd)
{
	struct addrinfo *temp;
	int flags_fcntl;
	
	temp = info;
	while(temp)
	{
		socketfd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
		if (socketfd == -1)
		{
			temp = temp->ai_next;
			continue ;
		}
		int activate = 1;
        if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &activate, sizeof(activate)) == -1)
		{
			close(socketfd);
			temp = temp->ai_next;
			continue ;    
		}
		if (bind(socketfd, temp->ai_addr, temp->ai_addrlen) == 0)
			break ;
		close(socketfd);
		temp = temp->ai_next;		
	}
	freeaddrinfo(info);
	if (!temp)
		return(true);
	flags_fcntl = fcntl(socketfd, F_GETFL);
	if (flags_fcntl == -1 || fcntl(socketfd, F_SETFL, flags_fcntl | O_NONBLOCK) == -1)
		return (close(socketfd), true);
	return(false);
}

bool ft_listener(std::string host, std::string listener, int &socketfd)
{
	struct addrinfo *info;
	struct addrinfo hints;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	if (getaddrinfo(host.c_str(),  listener.c_str(), &hints, &info))
	{
		std::cerr << "Error while opening socket listener 1." <<  std::endl;
		return (true);
	}
	if (ft_open_socket_listener(info, socketfd))
	{
		std::cerr << "Error while opening sockets listener 2." <<  std::endl;
		return (true);
	}
	if (listen(socketfd, SOMAXCONN) == -1)
	{
		close(socketfd);
		std::cerr << "Error while listening sockets listener 3." <<  std::endl;
		return (true);
	}
	return(false);
}

bool ft_construct_listener(std::map <int, Socket *> &map_socket, Config *config, int const &epollfd)
{
	Listen *temp_socket;
	struct epoll_event temp;
	size_t i = 0; 

	while(i < config->getServer().size())
	{
		size_t j = 0;
		while(j < config->getServer()[i].getListen().size())
		{
			temp_socket = new Listen();
			std::memset(&temp, 0, sizeof(temp));
			if(ft_listener(config->getServer()[i].getHost(), config->getServer()[i].getListen()[j], temp_socket->getFd()))
				return (delete temp_socket, true);
			temp_socket->setServerIndex(i);
			map_socket.insert(std::make_pair(temp_socket->getFd(), temp_socket));
			temp.data.fd = temp_socket->getFd();
			temp.events = EPOLLIN;
			if (epoll_ctl(epollfd, EPOLL_CTL_ADD, temp_socket->getFd(), &temp) == -1)
				return (true);
			j++;
		}
		i++;
	}
	return (false);
}
