#include "../../includes/http/HttpResponse.hpp"

HttpResponse::HttpResponse(){};

HttpResponse::~HttpResponse(){};

void HttpResponse::buildResponse(HttpRequest& request, ServerConfig &servConf)
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

void    HttpResponse::_buildGetResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{

}

void    HttpResponse::_buildPostResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{

}

void    HttpResponse::_buildDeleteResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{

}

void    HttpResponse::_buildErrorResponse(int error_code, ServerConfig &servConf, LocationConfig *location)
{

}

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