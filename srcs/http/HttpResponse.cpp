#include "../../includes/http/HttpResponse.hpp"
#include <cstdlib>
#include <fstream>
#include <fcntl.h>
#include <sstream>

HttpResponse::HttpResponse() : _status_code(200), _status_message("OK"), _headers(), _body("") {} // tmp

HttpResponse::~HttpResponse(){};

void HttpResponse::buildResponse(HttpRequest& request, ServerConfig &servConf)
{
   	if (request.getErrorCode() != 0)
    {
        std::cout << "[Debug] : ERROR "<< std::endl; // debug
        this->_buildErrorResponse(request.getErrorCode(), servConf, NULL);
		_response = _buildStringResponse();
		return ;
    }
    LocationConfig *location = servConf.matchLocation(request.getPath());
	if (location && !this->_isMethodAllowed(request.getMethod(), location)) // check droit
	{
		std::cout << "[Debug] : ERROR 405"<< std::endl; // debug
		this->_buildErrorResponse(405, servConf, location);
		_response = _buildStringResponse();
		return ;
	}
    if (request.getMethod() == "GET")
    {
        std::cout << "GET "<< std::endl; //debug
        this->_buildGetResponse(request, servConf, location);
    }
    else if (request.getMethod() == "POST")
    {
        std::cout << "POST "<< std::endl; //debug
        this->_buildPostResponse(request, servConf, location);
    }
    else if (request.getMethod() == "DELETE")
    {
        std::cout << "DELETE "<< std::endl; //debug
        this->_buildDeleteResponse(request, servConf, location);
    }
    else // method not allowed /
    {
        std::cout << "[Debug] : ERROR 501" << std::endl;
        this->_buildErrorResponse(501, servConf, location); // not found
    }
	_response = _buildStringResponse();
	return ;
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

void	HttpResponse::_buildGetResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{

}

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
			case 400: // location inutile
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
