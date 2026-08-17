#include "../../includes/http/HttpResponse.hpp"

HttpResponse::HttpResponse() : _state(NOT_BUILT),  _status_code(200), _status_message("OK"), _bytes_sent(0), _headers(), _body("") {}

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

// void	HttpResponse::_cgiBuild(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, Socket socket)
// {
// 	int fd;
// 	int pipe_in[2];
// 	int pipe_out[2];

// 	// creer pipe
// 	// socketpair (pipe_in[0], pipe_out[1])= merge socket;
// 	// fd = pipe_in[0] ou pipe_out[1]
// 	// in : recevoir
// 	// out :
// 	/*
// 		class socket :
// 				fd
// 				type : CGI
// 				parent_fd = fd du parent
// 		add au epoll
// 		add a la mapsocket
// 		fork()
// 		{
// 		tu fais ta magie
// 		}
// 	*/
// }
// int &epollfd, int &parent_fd, std::map<int, Socket> &map_socket
