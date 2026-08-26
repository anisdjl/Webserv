#include "../../../includes/socket/Cgi.hpp"
#include "../../../includes/socket/Connection.hpp"
#include "../../../includes/http/HttpResponse.hpp"

HttpResponse::HttpResponse() : _state(NOT_BUILT),  _status_code(200), _status_message("OK"), _bytes_sent(0), _headers(), _body(""), _isDone(false) {}

HttpResponse::~HttpResponse(){};

int         	HttpResponse::getStatusCode() const
{
	return this->_status_code;
}

void			HttpResponse::setState(ResponseState state)
{
	this->_state = state;
}

void			HttpResponse::add_bytes_sent(unsigned int bytes)
{
	this->_bytes_sent += bytes;
}

unsigned int	HttpResponse::get_bytes_sent() const
{
	return this->_bytes_sent;
}

void			HttpResponse::setResponse(const std::string& response)
{
	this->_response = response;
}

void			HttpResponse::setBody(const std::string& body)
{
	this->_body = body;
}

void			HttpResponse::setisDone(bool state)
{
	this->_isDone = state;
}

bool			HttpResponse::getisDone()
{
	return this->_isDone;
}

void HttpResponse::resetResponse()
{
	this->_status_code = 200;
	this->_status_message = "OK";
	this->_headers.clear();
	this->_body.clear();
	this->_state = NOT_BUILT;
	this->_response.clear();
	this->_bytes_sent = 0;
	this->_isDone = false;
}

static std::string	capitalize(std::string string)
{
	for (size_t i = 0; i < string.size(); ++i)
		string[i] = std::toupper(string[i]);
	return (string);
}

static std::string makeHeaderEnv(std::string key, std::string value)
{
	size_t colon = key.find(':');
	if (colon != std::string::npos)
		key.erase(colon);
	for (size_t i = 0; i < key.size(); ++i)
	{
		if (key[i] == '-')
			key[i] = '_';
		else
			key[i] = std::toupper(key[i]);
	}
	if (key != "CONTENT_TYPE" && key != "CONTENT_LENGTH")
		key = "HTTP_" + key;
	return key + "=" + value;
}

static char	*fillEnv(std::string string)
{
	char *env = new char[string.size() + 1];
	size_t i = 0;
	for (; i < string.size(); ++i)
		env[i] = string[i];
	env[i] = '\0';
	return (env);
}	

void    HttpResponse::_cgiBuild(HttpRequest& req, const ServerConfig &servConf, const LocationConfig *location, const int &epollfd, Connection &target, std::map<int, Socket *> &map_socket, std::string &req_path, std::string &index_path )
{
	int pid;
	int pipe_in[2];
	int pipe_out[2];

	if (access(req_path.c_str(), F_OK) != 0)
	{
		_buildErrorResponse(404, servConf, location);
		_response = _buildStringResponse();
		return ;
	}
	if (access(req_path.c_str(), R_OK) != 0)
	{
		_buildErrorResponse(403, servConf, location);
		_response = _buildStringResponse();
		return ;
	}

	Cgi	*new_cgi = new Cgi;


	char *path = getPath( index_path, location);
	if (!path)
	{
		_buildErrorResponse(403, servConf, location);
		_response = _buildStringResponse();
		return ;
	}
	char **argv = getArgv(req_path, path);
	char **env = getEnv(req, req_path, index_path);
	if (!env || !env[0])
	{
		_buildErrorResponse(500, servConf, location);
		_response = _buildStringResponse();
		return ;
	}
	if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
		throw std::runtime_error("Error: couldn't open pipes"); }

	pid = fork();
	if (pid < 0)
	{
		close (pipe_in[0]); close(pipe_in[1]); close(pipe_out[0]); close(pipe_out[1]);
		_buildErrorResponse(500, servConf, location);
		_response = _buildStringResponse();
		return;
	}
	
	new_cgi->setBeginExec();
    if (pid == 0)
    {
		dup2(pipe_out[0], STDIN_FILENO);
		close(pipe_out[0]); close(pipe_out[1]);

		dup2(pipe_in[1], STDOUT_FILENO);
		close(pipe_in[1]); close(pipe_in[0]);

		execve(path, argv, env);
		delete [] path;
		for (size_t i = 0; env[i] != NULL; ++i)
			delete [] env[i];
		delete [] env;
		for (size_t i = 0; argv[i] != NULL; ++i)
			delete [] argv[i];
		delete [] argv;
		exit(1);
	}
  		delete [] path;
		for (size_t i = 0; env[i] != NULL; ++i)
			delete [] env[i];
		delete [] env;
		for (size_t i = 0; argv[i] != NULL; ++i)
			delete [] argv[i];
		delete [] argv;
	close(pipe_in[1]);
	close(pipe_out[0]);
	struct epoll_event tmp1;
	tmp1.events = EPOLLIN;
	tmp1.data.fd = pipe_in[0];

	struct epoll_event tmp2;
	tmp2.events = EPOLLOUT;
	tmp2.data.fd = pipe_out[1];

	fcntl(pipe_out[1], F_SETFL, O_NONBLOCK);
	fcntl(pipe_in[0], F_SETFL, O_NONBLOCK);
	epoll_ctl(epollfd, EPOLL_CTL_ADD, pipe_in[0], &tmp1);

	new_cgi->setParentIndex(target.getFd());
	new_cgi->setType(CGI);
	new_cgi->setFd(pid);

	if (target.getHttpRequest().getBody().size() > 0)
	{
		// std::cout << "je passe ici le body n'est pas vide" << std::endl;
		new_cgi->setPipeOut(pipe_out[1]);
		map_socket[pipe_out[1]] = new_cgi;
		epoll_ctl(epollfd, EPOLL_CTL_ADD, pipe_out[1], &tmp2);
	}
	else
	{
		int fd_negative = -1;
		new_cgi->setPipeOut(fd_negative);
		close(pipe_out[1]);
	}
	new_cgi->setPipeIn(pipe_in[0]);
	new_cgi->setEpoll(epollfd);
	map_socket[pipe_in[0]] = new_cgi;
	new_cgi->setReqPath(req_path);
	this->_isDone = true;
}

char	**getEnv(HttpRequest &req, std::string &index_path, std::string &req_path)
{
	std::vector<std::string> env_var;
	env_var.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env_var.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env_var.push_back("SERVER_SOFTWARE=WeebServ/1.0");
	env_var.push_back("REDIRECT_STATUS=200");
	env_var.push_back("REQUEST_METHOD=" + capitalize(req.getMethod()));
	env_var.push_back("PATH_INFO=" + req.getPath());
	env_var.push_back("PATH_TRANSLATED=" + req_path);
	env_var.push_back("SCRIPT_NAME=" + index_path);
	env_var.push_back("SCRIPT_FILENAME=" + req_path);
	std::map<std::string, std::string>::const_iterator it = req.getHeader().find("cookie");
	if (it != (req.getHeader().end()))
		env_var.push_back("HTTP_COOKIE=" + it->second);


	if (!req.getQueryString().empty())
		env_var.push_back("QUERY_STRING=" + req.getQueryString());
	else
		env_var.push_back("QUERY_STRING=");

	if (req.getBody().size() > 0)
    {
        std::ostringstream ss;
        ss << req.getBody().size();
        env_var.push_back("CONTENT_LENGTH=" + ss.str());
    }
    for (std::map<std::string, std::string>::const_iterator it = req.getHeader().begin(); it != req.getHeader().end(); ++it)
		env_var.push_back(makeHeaderEnv(it->first, it->second));

	char **env = new char*[env_var.size() + 1];
	for (size_t i = 0; i < env_var.size(); ++i)
		env[i] = fillEnv(env_var[i]);
	env[env_var.size()] = NULL;

	return env;
}


char	*getPath(std::string &index_path, const LocationConfig *location) 
{
	if (access(index_path.c_str(), F_OK | R_OK) != 0)
		return (NULL);

	std::string extension;
	size_t pos_ex = index_path.rfind(".");
	if (pos_ex != std::string::npos)
		extension = index_path.substr(pos_ex);
	else
		throw std::out_of_range("Error: no extension found for the cgi"); 
	char	*path;
	std::string pathstr;

	for (std::map<std::string, std::string>::const_iterator it = (*location).getCgis().begin(); it != (*location).getCgis().end(); ++it)
	{
		if (it->first == extension)
		{
			pathstr = it->second.c_str();
			break;
		}
	}
	if (pathstr.empty())
		return (NULL);
	path = fillEnv(pathstr);
	return (path);
}

char	**getArgv(std::string &req_path, char *path)
{
	char **argv = new char*[3];

	argv[0] = fillEnv(path);
	argv[1] = fillEnv(req_path);
	argv[2] = NULL;
	
	return (argv);
}
