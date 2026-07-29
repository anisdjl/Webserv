#ifndef HTTPRESPONSE
#define HTTPRESPONSE

#include <iostream>
#include <map>
#include "HttpRequest.hpp"
#include "../config/Config.hpp"

class HttpResponse
{
    public:
		HttpResponse();
        ~HttpResponse();
        void								buildResponse(HttpRequest& request, Config &conf);
	private:
        int									_status_code;
        std::string							_status_message;
        std::map<std::string, std::string>	_headers;
        std::string							_body;
        t_location                          _matchLocation(std::string path);
        void                                _buildGetResponse(HttpRequest& req, Config &conf, t_location *location);
        void                                _buildPostResponse(HttpRequest& req, Config &conf, t_location *location);
        void                                _buildDeleteResponse(HttpRequest& req, Config &conf, t_location *location);
        void                                _buildErrorResponse(int error_code, Config &conf, t_location *location);
};

/*
    const char *response =
     "HTTP/1.1 200 OK\r\n"
     "Content-Type: text/plain\r\n"
     "Content-Length: 12\r\n"
     "\r\n"
     "Hello World";
    https://medium.com/@ryanradder0/building-an-http-server-from-scratch-a-journey-beyond-frameworks-28dc49e94a2a
*/

/*
    les fonctions necessaire devant traité:
        - get
        - post
        - delete
        - error : https://http.cat/
*/

#endif