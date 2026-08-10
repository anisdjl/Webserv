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

void			HttpResponse::buildResponse(HttpRequest& request, ServerConfig &servConf)
{
   	if (request.getErrorCode() != 0)
    {
        this->_buildErrorResponse(request.getErrorCode(), servConf, NULL);
		_response = _buildStringResponse();
		return ;
    }
	if (request.getVersion() != "HTTP/1.1")
	{
		this->_buildErrorResponse(505, servConf, NULL);
		_response = _buildStringResponse();
		return ;
	}
    LocationConfig *location = servConf.matchLocation(request.getPath());
	if (location && !this->_isMethodAllowed(request.getMethod(), location)) // check droit
	{
		this->_buildErrorResponse(405, servConf, location);
		_response = _buildStringResponse();
		return ;
	}
    if (request.getMethod() == "GET")
        this->_buildGetResponse(request, servConf, location);
    else if (request.getMethod() == "POST")
        this->_buildPostResponse(request, servConf, location);
    else if (request.getMethod() == "DELETE")
        this->_buildDeleteResponse(request, servConf, location);
    else
		this->_buildErrorResponse(501, servConf, location); // not found
	_response = _buildStringResponse();
}

// for (size_t i = 0; i < location->getMethods().size(); i++)

std::string		HttpResponse::_buildStringResponse()
{
    std::stringstream ss;

    ss << "HTTP/1.1 " << this->_status_code 
		<< " " << this->_status_message << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
         it != this->_headers.end(); ++it)
    {
        ss << it->first << ": " << it->second << "\r\n";
    }
    ss << "\r\n";
    ss << this->_body;
	// /r pour la norme http
    return (ss.str());
}

void			HttpResponse::_buildAutoIndexResponse(std::string path, HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	std::vector<std::string> file;

	DIR *dir= opendir(path.c_str());
	if (!dir)
		return (_buildErrorResponse(500, servConf, location));
	dirent *pdir;
	while((pdir = readdir(dir)))
		file.push_back(pdir->d_name);
	closedir(dir);
	this->_body = "<html>\n"
						"<head><title>" + req.getPath() + "</title></head>\n"
						"<body>\n"
							"<hr>"
							"<pre>"
							"<h1> " + req.getPath() + " </h1>\n";
	for (std::vector<std::string>::const_iterator it = file.begin(); 
		it !=  file.end(); ++it)
		this->_body += "<a href=\"" + *it + "\">" + *it +"</a>\n";
	this->_body +=	"</pre>";
	this->_body += "</body>\n";
	this->_body +=	"</html>";
	
	this->_status_code = 200;
    this->_status_message = "OK";
	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Content-Type", "text/html"));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
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
