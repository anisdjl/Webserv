#ifndef HTTPRESPONSE
#define HTTPRESPONSE

#include <iostream>
#include <map>

class HttpResponse
{
    public:
		HttpResponse();
        ~HttpResponse();
	private:
        int									_status_code;
        std::string							_status_message;
        std::map<std::string, std::string>	_headers;
        std::string							_body;
};

/*
const char *response_405 =
    "HTTP/1.1 405 Method Not Allowed\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 18\r\n"
    "\r\n"
    "Method Not Allowed";
*/

#endif