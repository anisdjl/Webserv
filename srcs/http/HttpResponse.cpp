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

void	HttpResponse::_cgiBuild(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, Socket socket)
{
	int fd;
	int pipe_in[2]; // sends the body to the child
	int pipe_out[2]; // recives the result from the child
	int	save_fd_out = dup(STDOUT_FILENO);
	int save_fd_in = dup(STDIN_FILENO);


	if (pipe(pipe_in) == -1)
		throw std::runtime_error("Error: couldn't open pipes");
	if (pipe(pipe_out) == -1)
		throw std::runtime_error("Error: couldn't open pipes");


	dup2(pipe_out[1], STDOUT_FILENO); // on exrit dedans
	close(pipe_out[1]);
	
	dup2(pipe_in[0], STDIN_FILENO); // on lit depuis

	write(0, req.getBody().c_str(), req.getBody().size());
	fd = fork();
	if (fd == 0) // on est dans le child et donc on doit rcupe
	{
		dup2(pipe_out[0], STDIN_FILENO); // on lit depuis
		close(pipe_out[0]); close(pipe_out[1]);

		dup2(pipe_in[1], STDOUT_FILENO); // on ecrit dedans
		close(pipe_in[1]);

		char **env = getEnv(req, servConf, location);
		char *path = getPath(req, servConf, location);
		char **argv = getArgv(req, servConf, location);

		// je dois me mettre dans le dossier du fichier a executer
		// je dois recup l'env
		// je dois join le path
		// je dois faire le tab de arg


		execve(path, argv, env); // path is the path to the interpreter / argv is the command to execute, so path + file / env is all the informations of request
	}
	close(pipe_in[1]);
	close(pipe_in[0]);
	close(pipe_out[0]);
	dup2(STDOUT_FILENO, save_fd_out);
	
}


char	**getEnv(HttpRequest &req, ServerConfig &servConf, LocationConfig *location)
{

}

char	*getPath(HttpRequest &req, ServerConfig &servConf, LocationConfig *location)
{

}

char	**getArgv(HttpRequest &req, ServerConfig &servConf, LocationConfig *location)
{

}

// int &epollfd, int &parent_fd, std::map<int, Socket> &map_socket
