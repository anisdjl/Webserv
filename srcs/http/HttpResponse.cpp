#include "../../includes/http/HttpResponse.hpp"
#include <cstdlib>
#include <fstream>
#include <fcntl.h>

HttpResponse::HttpResponse(){};

HttpResponse::~HttpResponse(){};

std::string HttpResponse::buildResponse(HttpRequest& request, ServerConfig &servConf)
{
    /* 
        trouver la location == au path de la config, comp a la request
        Si aucun location trouver alors settings par defaut qui s'applique
    */
   	if (request.getMethod() == "ERROR")
    {
        std::cout << "ERROR "<< std::endl; // debug
        this->_buildErrorResponse(400, servConf, NULL);
    }
    LocationConfig *location = servConf.matchLocation(request.getPath());
	// poser le 405 not allowed

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
        std::cout << "erreur ici" << std::endl;
        this->_buildErrorResponse(405, servConf, location); // not found
        return ;
    }
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
		std::string   line;
		std::ifstream infile("www/html/Error.html"); // a modifier
		if (!infile.is_open())
		{
			this->_body = "<html>\n"
							"<head><title>404 Not Found</title></head>\n"
							"<body>\n<h1>404 Not Found</h1>\n</body>\n"
							"<html>";
		}
		else
		{
			while (std::getline(infile, line))
			{
				line += "\n";
				this->_body += line;
			}
			infile.close();
		}
		this->_status_code = error_code;
		switch (error_code)
		{
			case 400: // location inutile
				this->_status_message = "400 Bad Request";
				break;
			case 403:
				this->_status_message = "403 Forbidden";
				break;
			case 404:
				this->_status_message = "404 Not Found";	
				break;
			case 405: // location est utile ici
				this->_status_message = "405 Method Not Allowed";
				break;
			case 413:
				this->_status_message = "413 Payload Too Large";
				break;
			default:
				break;
		}
		/* header */
		this->_headers.insert(std::make_pair("Server: ", "WeebServ"));
		this->_headers.insert(std::make_pair("Content-Type: ", "text/html"));
		this->_headers.insert(std::make_pair("Content-Length: ", "taille"));
		this->_headers.insert(std::make_pair("Connnexion: ", "close"));
}

/*
	for(std::map<std::string, std::string >::const_iterator it = _header.begin();
		it != _header.end(); ++it)
	{
		if (it->first == key)
			return it->second; 
	}
	return "";
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