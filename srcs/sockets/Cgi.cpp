#include "../../includes/Webserv.hpp"
#include "../../includes/socket/Socket.hpp"
#include "../../includes/socket/Cgi.hpp"
#include "../../includes/socket/Connection.hpp"

static void	buildFinalResponse(std::map<int, Socket *> &map_socket, Cgi &target, Connection &parent, Config *config)
{
	struct epoll_event event2;
	event2.data.fd = target.getParentIndex();
	event2.events = EPOLLOUT;

	checkContentTypeScript(parent);
	std::ostringstream oss;
	oss << parent.getHttpResponse().getBody().size();
	parent.getHttpResponse().setHeader("Content-Length", oss.str());

	parent.getHttpResponse().buildResponse(parent, config->getServer()[parent.getServerIndex()], map_socket, target.getEpoll());
	if (parent.getHttpResponse().getState() == BUILT)
		epoll_ctl(target.getEpoll(), EPOLL_CTL_MOD, target.getParentIndex(), &event2);

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

static void	fillBody(Cgi &target, Connection &parent)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read = 1;

	while (bytes_read > 0)
	{
		bytes_read = read(target.getPipeIn(), buffer, BUFFER_SIZE);
		if (bytes_read > 0)
			parent.getHttpResponse().addBody(buffer, static_cast<size_t>(bytes_read));
	}
	if (bytes_read == 0)
		target.setStdoutDone(true);
}

bool	ft_cgi_in(std::map<int, Socket *> &map_socket, Cgi &target, Config *config)
{
	std::map<int, Socket *>::iterator it = map_socket.find(target.getParentIndex());
	if (it == map_socket.end())
		return (true);
	(void)config;

	char buffer[BUFFER_SIZE + 1];
	ssize_t bytes_read = 0;

	Connection &parent = dynamic_cast<Connection &>(*(it->second));

	std::memset(buffer, 0, BUFFER_SIZE + 1);
	bytes_read = read(target.getPipeIn(), buffer, BUFFER_SIZE);

	if (bytes_read > 0)
		parent.getHttpResponse().addBody(buffer, static_cast<size_t>(bytes_read));
	else if (bytes_read == 0)
	{
		target.setStdoutDone(true);
		if (target.getChildDone())
			buildFinalResponse(map_socket, target, parent, config);
	}
	return (false);
}

bool	ft_cgi_out(std::map<int, Socket *> &map_socket, Cgi &target, Config *config)
{
	std::map<int, Socket *>::iterator it = map_socket.find(target.getParentIndex());
	if (it == map_socket.end())
		return (true);
	(void)config;
	Connection &parent = dynamic_cast<Connection &>(*(it->second));
	
	int	fd_write = target.getPipeOut();
	const std::string &body = parent.getHttpRequest().getBody();
	ssize_t written_bytes = target.getBodyWritten();
	ssize_t	body_size = parent.getHttpRequest().getBody().size();
	ssize_t	res = write(fd_write, body.c_str() + written_bytes, body_size - written_bytes);
	
	if (res > 0)
		target.addWrittenBytes(res);
	if (res == -1)
		return (false);
	if (target.getBodyWritten() >= body_size)
	{
		close(fd_write);
		epoll_ctl(target.getEpoll(), EPOLL_CTL_DEL, fd_write, NULL);
		map_socket.erase(fd_write);
		int i = -1;
		target.setPipeOut(i);
	}
	return (false);
}

void	ft_cgi_hup(std::map<int, Socket *> &map_socket, Cgi &target, Config *config)
{
	int status;
	std::map<int, Socket *>::iterator it = map_socket.find(target.getParentIndex());
	if (it == map_socket.end())
		return ;
	Connection &parent = dynamic_cast<Connection &>(*(it->second));
	
	pid_t res = waitpid(target.getChildFd(), &status, WNOHANG);
	if (res == 0)
		return ;

	if (res > 0)
	{
		if ((WIFEXITED(status) && WEXITSTATUS(status) != 0) || WIFSIGNALED(status))
			parent.getHttpRequest().setError(500);
	}
	else
		parent.getHttpRequest().setError(500);

	target.setChildDone(true);
	fillBody(target, parent);
	if (!target.getStdoutDone())
		return ;
	buildFinalResponse(map_socket, target, parent, config);
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

void	checkContentTypeScript(Connection &client)
{
	std::string	body = client.getHttpResponse().getBody();
	std::string	contentType;
	size_t		size = 4;
	std::string	headers;
	
	size_t	pos = body.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		pos = body.find("\n\n");
		size = 2;
	}
	
	if (pos == std::string::npos)
	{
		contentType = "text/html";
		client.getHttpResponse().setBody(body);
	}
	else
	{
		headers = body.substr(0, pos);
		client.getHttpResponse().setBody(body.substr(pos + size));
	}
	bool	foundContentType = false;
	std::stringstream	ss(headers);
	std::string	line;
	while(getline(ss, line))
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		
		size_t	pos_delim = line.find(":");
		if (pos_delim != std::string::npos)
		{
			std::string	key = line.substr(0, pos_delim);
			std::string	value = line.substr(pos_delim +1);
			
			size_t first_char = value.find_first_not_of(" \t");
			if (first_char != std::string::npos)
				value = value.substr(first_char);
			if (key == "content-type" || key == "Content-Type")
			{
				contentType = value;
				foundContentType = true;
			}
			// if (key == "status" || key == "Status")
			// {
			// 	int code = std::atoi(value.c_str());
			// 	if (code >= 100 && code < 600)
			// 		client.getHttpRequest().setError(code);
			// }
			client.getHttpResponse().setHeader(key, value);
		}
	}
	if (!foundContentType)
		client.getHttpResponse().setHeader("Content-Type", contentType);
	return ;
}
