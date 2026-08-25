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

// static void	display(char **env)
// {
// 	int i = 0;
// 	while(env[i])
// 	{
// 		std::cout << env[i] << std::endl;
// 		i++;
// 	}
// 	return ;
// }

void    HttpResponse::_cgiBuild(HttpRequest& req, const ServerConfig &servConf, const LocationConfig *location, const int &epollfd, Connection &target, std::map<int, Socket *> &map_socket)
{
	int pid;
	int pipe_in[2];
	int pipe_out[2];

	std::string root = location->getRoot();
	std::string req_path = root + req.getPath();
	req_path = _clearPathGarbage(req_path);


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

	char **env = getEnv(req, servConf, location, req_path);
	char *path = getPath(req, servConf, location);
	if (!path)
	{
		_buildErrorResponse(403, servConf, location);
		_response = _buildStringResponse();
		return ;
	}
	char **argv = getArgv(req, servConf, location, path);
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
	std::cout << req.getVersion() <<std::endl;
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

char	**getEnv(HttpRequest &req, const ServerConfig &servconf, const LocationConfig *location, std::string &req_path)
{
	std::vector<std::string> env_var;
	std::cout << req.getPath() << std::endl;
	(void)servconf; (void)location;
	env_var.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env_var.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env_var.push_back("SERVER_SOFTWARE=WeebServ/1.0");
	env_var.push_back("REDIRECT_STATUS=200");
	env_var.push_back("REQUEST_METHOD=" + capitalize(req.getMethod()));
	env_var.push_back("PATH_INFO=" + req.getPath());
	env_var.push_back("PATH_TRANSLATED=" + req_path);
	env_var.push_back("SCRIPT_NAME=" + req.getPath());
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
	// (void)servconf; (void)location;
	// char	**env;
	// size_t	size_of_env = req.getHeader().size() + 2;
	// if (req.getHeader().find("Cookie") != req.getHeader().end())
	// 	env = new char*[size_of_env + 1];
	// else
	// 	env = new char*[size_of_env];
	// std::string capital = capitalize(req.getMethod());
	// std::string	method = "REQUEST_METHOD=" + capital;

	// env[0] = fillEnv(method);
	// size_t	i = 1;
	// for (std::map<std::string, std::string>::const_iterator it = req.getHeader().begin(); it != req.getHeader().end(); ++it)
	// {
	// 	std::string header = makeHeaderEnv(it->first, it->second);
	// 	// std::cout << "le header " << header << std::endl;
	// 	// std::cout << i << " tour de boucle" << std::endl;
	// 	env[i] = fillEnv(header);
	// 	i++;
	// }
	// env[i] = NULL;
	// //display(env);
	// return (env);
}

char	*getPath(HttpRequest &req, const ServerConfig &servconf, const LocationConfig *location) // ici je vais aussi recevoir la map des sockets, le epollfd, et un objet cgi pour pouvoir les neregistrer
{
	(void)servconf;
	std::string filename = location->getRoot() + req.getPath();
	
	// std::cout << "je suis dans getpath" << std::endl;
	if (access(filename.c_str(), F_OK | R_OK) != 0)
	{
		std::cout << "fichier inaccessible" << std::endl;
		return (NULL);
		// return (_buildError(403, servconf, location));
		// je dois return null je pense
	}

	std::string extension;
	size_t pos_ex = req.getPath().rfind(".");
	if (pos_ex != std::string::npos)
		extension = req.getPath().substr(pos_ex);
	else
		throw std::out_of_range("Error: no extension found for the cgi"); // pas sur de faire ca sinon ca va couper le server je pense qu'on renverra une erreur correcte

	// std::cout << "extension " << extension << std::endl;
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
	// std::cout << pathstr << std::endl;
	path = fillEnv(pathstr);

	// std::cout << "the final path " << path << std::endl;
	return (path);
}


// char **getArgv(HttpRequest &req, const ServerConfig &servconf, const LocationConfig *location, char *path)
// {
//     (void)servconf;
//     std::string pathstr = path;
//     size_t pos = pathstr.find_last_of("/");
//     std::string bin_name = (pos != std::string::npos) ? pathstr.substr(pos + 1) : pathstr;

//     // Pour php-cgi : argv contient seulement le binaire
//     if (bin_name == "php-cgi" || bin_name == "php")
//     {
//         char **argv = new char*[2];
//         argv[0] = fillEnv(bin_name);
//         argv[1] = NULL;
//         return argv;
//     }

//     // Pour python3 ou les autres interpréteurs : argv = [interpréteur, fichier_cible, NULL]
//     char **argv = new char*[3];
//     argv[0] = fillEnv(bin_name);
//     argv[1] = fillEnv(location->getRoot() + req.getPath());
//     argv[2] = NULL;
//     return argv;
// }

char	**getArgv(HttpRequest &req, const ServerConfig &servconf, const LocationConfig *location, char *path)
{
	char **argv = new char*[3];
	(void)servconf; (void)req;
	
	std::string pathstr = path;

	size_t	pos = pathstr.find_last_of("/");

	if (pos != std::string::npos)
		argv[0] = fillEnv(pathstr.substr(pos + 1));
	else
		throw std::out_of_range("Error: no extension found for the cgi");

	std::string	fullPath = location->getRoot() + req.getPath();
	argv[1] = fillEnv(fullPath);
	argv[2] = NULL;
	
	return (argv);
}
