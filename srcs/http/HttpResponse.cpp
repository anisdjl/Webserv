#include "../../includes/http/HttpResponse.hpp"
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

HttpResponse::HttpResponse() : _status_code(200), _status_message("OK"), _headers(), _body("") {} // tmp

HttpResponse::~HttpResponse(){};

void HttpResponse::buildResponse(HttpRequest& request, ServerConfig &servConf)
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
    else // method not allowed /
        this->_buildErrorResponse(501, servConf, location); // not found
	_response = _buildStringResponse();
}

bool	HttpResponse::_isMethodAllowed(std::string methode, LocationConfig *location)
{
	for (std::vector<std::string>::const_iterator it = location->getMethods().begin();
		it != location->getMethods().end(); ++it)
	{
		if (*it == methode)
			return (true);
	}
	return (false);
}
// for (size_t i = 0; i < location->getMethods().size(); i++)

std::string HttpResponse::_buildStringResponse()
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

std::string	HttpResponse::_findContentType(std::string path)
{
	if (path.empty())
		return ("application/octet-stream");
	size_t pos = path.rfind(".");
	if (pos == std::string::npos || pos == 0)
		return ("application/octet-stream");
	std::string	extension = path.substr(pos + 1);
	
	if (extension == "html")
		return ("text/html");
	else if (extension == "css")
		return ("text/css");
	else if (extension == "js")
		return ("text/javascript");
	else if (extension == "json")
		return ("application/json");
	else if (extension == "png")
		return ("image/png");
	else if (extension == "jpeg" || extension == "jpg")
		return ("image/jpeg");
	else if (extension == "svg")
		return ("image/svg+xml");
	else if (extension == "webp")
		return ("image/webp");
	else if (extension == "avif")
		return ("image/avif");
	else if (extension == "gif")
			return ("image/gif");
	else if (extension == "pdf")
		return ("application/pdf");
	else if (extension == "txt")
		return ("text/plain");
	else if (extension == "zip")
		return ("application/zip");
	return ("application/octet-stream");
}

void	HttpResponse::_buildGetResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	std::string	root;
	std::string req_path;
	if (location && !location->getRoot().empty())
		root = location->getRoot();
	else
		root = servConf.getRoot();
	size_t 		pos = req_path.rfind(".");
	if (req.getPath().size() == 1 && req.getPath()[0] == '/')
	{
		if (root.back() == '/')
			req_path = root + "index.html";
		else
			req_path = root + "/index.html";
	}
	else if (pos != 2 && pos == std::string::npos 
			&& req.getPath()[0] == '/' && req.getPath().back() == '/')
		req_path = root + "index.html";
	else
		req_path = root + req.getPath();
	if (access(req_path.c_str(), F_OK) == -1)
		return (_buildErrorResponse(404, servConf, location));
	if (access(req_path.c_str(), R_OK) == -1)
		return (_buildErrorResponse(403, servConf, location));
	this->_status_code = 200;
	this->_status_message = "OK";

	// if cgi => fonction vers cgiBuild

	std::ifstream			infile(req_path.c_str(), std::ios::binary | std::ios::in | std::ios::ate);
	std::ifstream::pos_type	size;
	if (infile)
	{
    	size = infile.tellg();
		if (size > 0)
		{
			this->_body.resize(size);
			infile.seekg(0, std::ios::beg);
			infile.read(&this->_body[0], size);
		}
	}
	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Content-Type", _findContentType(req.getPath())));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
}

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
// cas manquant :
/*
	manque cas avec cgi
	auto index
	/ et /img/
*/

/*
	GET HTTP/1.1
	Host: www.aaaa.com
	User-Agent: Mozilla/5.0
	Accept: text/html,application/xhtml+xml,image/webp,application/json
	Accept-Language: fr-FR
	Connection: keep-alive
	Cookie: session_id=abc123
*/

void	HttpResponse::_buildPostResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{

}

void	HttpResponse::_buildDeleteResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{

}

void	HttpResponse::_buildErrorResponse(int error_code, ServerConfig &servConf, LocationConfig *location)
{
	this->_status_code = error_code;
	switch (error_code)
	{
		case 400: // erreur dans le parsing
			this->_status_message = "Bad Request";
			break;
		case 403:
			this->_status_message = "Forbidden";
			break;
		case 404:
			this->_status_message = "Not Found";	
			break;
		case 405: // location est utile ici
			this->_status_message = "Method Not Allowed";
			break;
		case 413:
			this->_status_message = "Payload Too Large";
			break;
		case 501:
			this->_status_message = "Not Implemented";
			break;
		case 505:
			this->_status_message = "Version Not Supported";
			break;
		default:
			this->_status_message = "Internal Server Error";
			break;
	}
	std::string		root;
	std::string		html_path("");
	std::map<int, std::string >::const_iterator	it = servConf.findErrorPage(error_code);

	if (it != servConf.getErrorPage().end())
	{
		if (location && !location->getRoot().empty())
			root = location->getRoot();
		else
			root = servConf.getRoot();
		html_path = root + it->second;
	}

	std::ifstream	infile(html_path.c_str());
	// this->_body.clear();
	if (!infile.is_open())
	{
			this->_body = "<html>\n"
								"<head><title>" + this->_status_message + "</title></head>\n"
								"<body>\n"
									"<h1>" + this->_status_message + "</h1>\n"
								"</body>\n"
							"</html>";
	}
	else
	{
		std::string		line;
		while (std::getline(infile, line))
		{
			line += "\n";
			this->_body += line;
		}
		infile.close();
	}
	/* header */
	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.clear(); 
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Content-Type", "text/html"));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "close"));
}

void HttpResponse::resetResponse()
{
	this->_status_code = 200;
	this->_status_message = "OK";
	this->_headers.clear();
	this->_body.clear();
	this->_state = NOT_BUILT;
	this->_response.clear();
}

ResponseState	HttpResponse::getState()
{
	return this->_state;
}

void	HttpResponse::setState(ResponseState state)
{
	this->_state = state;
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
*/

/*
std::map<int, std::string >::const_iterator ServerConfig::findErrorPage(int key) const
{
    return (_error_page.find(key));
}
*/

// Exemple possible de request http :
/*
	POST /cgi-bin/upload.py?user=42&action=save HTTP/1.1\r\n
	Host: localhost:8080\r\n
	User-Agent: Mozilla/5.0 (X11; Linux x86_64)\r\n
	Content-Type: application/x-www-form-urlencoded\r\n
	Content-Length: 27\r\n
	Cookie: session_id=abc123xyz\r\n
	\r\n
	name=JohnDoe&age=25&status=ok
*/

/* exemple possible de reponse :

    HTTP/1.1 404 Not Found
    Content-Type: text/html
    Content-Length: 149
    Connection: close

    <html>
    <head><title>404 Not Found</title></head>
    <body>
    <h1>404 Not Found</h1>
    <hr><center>Webserv/1.0</center>
    </body>
    </html>
*/
