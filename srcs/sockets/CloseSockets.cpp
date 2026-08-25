#include "../../includes/socket/Socket.hpp"
#include "../../includes/socket/Cgi.hpp"

void ft_close_socket(std::map<int, Socket *> &map_socket, int target_fd, const int &epollfd)
{
	Socket *target = map_socket.find(target_fd)->second;

	if (target)
	{
		epoll_ctl(epollfd, EPOLL_CTL_DEL, target_fd, NULL);
		delete target;
		map_socket.erase(target_fd);
		close(target_fd);
	}
}

void ft_close_cgi(std::map<int, Socket *> &map_socket, int target_fd, const int &epollfd)
{
	Cgi *target = dynamic_cast<Cgi *>(map_socket.find(target_fd)->second);

	
	if (target)
	{
		int pipe_in = target->getPipeIn();
		int pipe_out = target->getPipeOut();
		pid_t pid = target->getChildFd();
		
		waitpid(pid, NULL, WNOHANG);
		if (pipe_in != -1)
		{
			epoll_ctl(epollfd, EPOLL_CTL_DEL, pipe_in, NULL);
			close(pipe_in);
			map_socket.erase(pipe_in);
		}
		if (pipe_out != -1)
		{
			epoll_ctl(epollfd, EPOLL_CTL_DEL, pipe_out, NULL);
			close(pipe_out);
			map_socket.erase(pipe_out);
		}
		delete target;
	}
}

void ft_close_all_sockets(std::map<int, Socket *> &map_socket, const int &epollfd)
{
	std::vector <int> fd_to_destroy;

	while(map_socket.size() > 0)
	{
		if (map_socket.begin()->second->getType() != CGI)
			ft_close_socket(map_socket, map_socket.begin()->second->getFd(), epollfd);
		else
			ft_close_cgi(map_socket, map_socket.begin()->second->getFd(), epollfd);
	}
	close(epollfd);
}
