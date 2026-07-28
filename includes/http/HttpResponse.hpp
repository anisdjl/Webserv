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
        void                                _buildGetResponse();
        void                                _buildPostResponse();
        void                                _buildDeleteResponse();
        void                                _buildErrorResponse();
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