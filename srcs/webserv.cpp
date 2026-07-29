#include "webserv.hpp"

bool ft_webserv(Config *config)
{
	std::vector<int> listeners(config->listen.size());
	std::vector<struct pollfd> fds;
	struct pollfd temp;
	int i = 0; 

	while(i < config->listen.size())
	{
		
		if(ft_listener(config->listen[i], temp.fd))
			return (true);
		listeners.push_back(temp.fd);
		temp.events = POLLIN;
		temp.revents = 0;
		fds.push_back(temp);
		i++;
	}
	return (false);
}
