#include "../../includes/http/HttpResponse.hpp"

HttpResponse::HttpResponse(){};

HttpResponse::~HttpResponse(){};

void HttpResponse::buildResponse(HttpRequest& request, Config &conf)
{
    /* 
        trouver la location == au path de la config, comp a la request
        Si aucun location trouver alors settings par defaut qui s'applique
    */
   	if (request.getMethod() == "ERROR")
    {
        std::cout << "ERROR "<< std::endl; // debug
        this->_buildErrorResponse(400, conf, NULL);
    }
	t_location   *location = conf.matchLocation(request.getPath());

	// poser le 405 not allowed

    if (request.getMethod() == "GET")
    {
        std::cout << "GET "<< std::endl; //debug
        this->_buildGetResponse(request, conf, location);
    }
    else if (request.getMethod() == "POST")
    {
        std::cout << "POST "<< std::endl; //debug
        this->_buildPostResponse(request, conf, location);
    }
    else if (request.getMethod() == "DELETE")
    {
        std::cout << "DELETE "<< std::endl; //debug
        this->_buildDeleteResponse(request, conf, location);
    }
    else // method not allowed /
    {
        std::cout << "erreur ici" << std::endl;
        this->_buildErrorResponse(405, conf, location); // not found
        return ;
    }
}

void    HttpResponse::_buildGetResponse(HttpRequest& req, Config &conf, t_location *location)
{

}

void    HttpResponse::_buildPostResponse(HttpRequest& req, Config &conf, t_location *location)
{

}

void    HttpResponse::_buildDeleteResponse(HttpRequest& req, Config &conf, t_location *location)
{

}

void    HttpResponse::_buildErrorResponse(int error_code, Config &conf, t_location *location)
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