#include "../../includes/http/HttpResponse.hpp"

HttpResponse::HttpResponse() : _status_code(200), _status_message("OK"), _bytes_sent(0), _headers(), _body("") {}

HttpResponse::~HttpResponse(){};

std::string		HttpResponse::getResponse() const
{
	return (this->_response);
}

std::string		HttpResponse::getBody() const
{
	return (this->_body);
}

int         	HttpResponse::getStatusCode() const
{
	return (this->_status_code);
}

ResponseState	HttpResponse::getState()
{
	return this->_state;
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

void HttpResponse::resetResponse()
{
	this->_status_code = 200;
	this->_status_message = "OK";
	this->_headers.clear();
	this->_body.clear();
	this->_state = NOT_BUILT;
	this->_response.clear();
	this->_bytes_sent = 0;
}

/*
	Location
	location->root // possible

	ServConf
	getErrorCodePage
	findErrorPage
	matchLocation

	il faut add la verife avant le build

	parser :
	400
	413

	Traiter les chemins absolues ?
	manque un / sur le debut ? get
	traiter les doubles /
	root vide partout
	manque cas avec cgi
*/

static std::string	capitalize(std::string string)
{
	for (size_t i = 0; i < string.size(); ++i)
		string[i] = std::toupper(string[i]);
	return (string);
}

static std::string makeHeaderEnv(std::string key, std::string value)
{
	std::string	header;

	for (size_t i = 0; i < key.size(); ++i)
	{
		if (key[i] == '-')
			key[i] = '_';
		else
			key[i] = std::toupper(key[i]);
	}

	header = key + "=" + value;
	return (header);
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

static void	display(char **env)
{
	int i = 0;
	while(env[i])
	{
		std::cout << env[i] << std::endl;
		i++;
	}
	return ;
}

void    _cgiBuild(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, int &epollfd)
{
	int fd;
	int pipe_in[2];
	int pipe_out[2];

	// je cree un objet cgi que je vis append a la map socket, dans cet objet je vais mettre les deuix bouts du pipe 
	if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
		throw std::runtime_error("Error: couldn't open pipes"); }

	fd = fork();

	write(pipe_out[1], req.getBody().c_str(), req.getBody().size());

    if (fd == 0)
    {
		dup2(pipe_out[0], STDIN_FILENO);
		close(pipe_out[0]); close(pipe_out[1]);

		dup2(pipe_in[1], STDOUT_FILENO);
		close(pipe_in[1]); close(pipe_in[0]);

		char **env = getEnv(req, servConf, location);
		char *path = getPath(req, servConf, location);
		char **argv = getArgv(req, servConf, location, path); 

		if (env == NULL)
			return (req.setError(0));
		if (path == NULL)
		// check if anything is null et set une erreur en fonction et ensuite mettre l'etat a pas de chance
		execve(path, argv, env);
		delete path;
		for (size_t i = 0; env[i] != NULL; ++i)
			delete [] env[i];
		delete [] env;
		for (size_t i = 0; argv[i] != NULL; ++i)
			delete [] argv[i];
		delete [] argv;
		exit(1);
	}

	struct epoll_event tmp1;
	tmp1.events = EPOLLIN;
	tmp1.data.fd = pipe_in[0];

	struct epoll_event tmp2;
	tmp2.events = EPOLLOUT;
	tmp2.data.fd = pipe_out[1];

	// mettre en non bloquant
	fcntl(pipe_out[1], F_SETFL, O_NONBLOCK);
	fcntl(pipe_in[0], F_SETFL, O_NONBLOCK);
	epoll_ctl(epollfd, EPOLL_CTL_ADD, pipe_in[0], &tmp1); epoll_ctl(epollfd, EPOLL_CTL_ADD, pipe_out[1], &tmp2);
	// register in epoll
  
	// je pense pas les fermer ici mais plus dans cgi_in
	// close(pipe_out[1]);
    // close(pipe_in[0]);
}

char	**getEnv(HttpRequest &req, ServerConfig &servconf, LocationConfig *location)
{
	(void)servconf; (void)location;

	size_t	size_of_env = req.getHeader().size() + 2;
	char	**env = new char*[size_of_env];
	std::string capital = capitalize(req.getMethod());
	std::string	method = "REQUEST_METHOD=" + capital;

	env[0] = fillEnv(method);
	size_t	i = 1;
	for (std::map<std::string, std::string>::const_iterator it = req.getHeader().begin(); it != req.getHeader().end(); ++it)
	{
		std::string header = makeHeaderEnv(it->first, it->second);
		env[i] = fillEnv(header);
		i++;
	}
	env[i] = NULL;
	display(env);
	return (env);
}

char	*getPath(HttpRequest &req, ServerConfig &servconf, LocationConfig *location) // ici je vais aussi recevoir la map des sockets, le epollfd, et un objet cgi pour pouvoir les neregistrer
{
	(void)servconf;
	const char	*filename = req.getPath().c_str();
	
	if (access(filename, F_OK | R_OK) != 0)
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

	std::cout << "extension " << extension << std::endl;
	char	*path;
	std::string pathstr;

	for (std::map<std::string, std::string>::iterator it = (*location).getCgis().begin(); it != (*location).getCgis().end(); ++it)
	{
		if (it->first == extension)
		{
			pathstr = it->second.c_str();
			break;
		}
	}
	path = fillEnv(pathstr);

	std::cout << "the final path " << path << std::endl;
	return (path);
}

char	**getArgv(HttpRequest &req, ServerConfig &servconf, LocationConfig *location, char *path)
{
	char **argv = new char*[3];
	(void)servconf; (void)req;
	
	std::string pathstr = path;
	size_t	pos = pathstr.find_last_of("/");

	if (pos != std::string::npos)
		argv[0] = fillEnv(pathstr.substr(pos));
	else
		throw std::out_of_range("Error: no extension found for the cgi"); // pas sur de faire ca sinon ca va couper le server je pense qu'on renverra une erreur correcte

	std::string	fullPath = location->getPath() + pathstr;
	argv[1] = fillEnv(fullPath);
	argv[2] = NULL;
	std::cout << "full path " << fullPath << std::endl;
	
	return (argv);
}