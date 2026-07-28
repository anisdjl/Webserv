#include "../../includes/http/HttpResponse.hpp"

HttpResponse::HttpResponse(){};

HttpResponse::~HttpResponse(){};

void HttpResponse::buildResponse(HttpRequest& request, Config &conf)
{
    if (request.getMethod() == "GET")
    {
        std::cout << "GET "<< std::endl; //debug
        
    }
    else if (request.getMethod() == "POST")
    {
        std::cout << "POST "<< std::endl; //debug

    }
    else if (request.getMethod() == "DELETE")
    {
        std::cout << "DELETE "<< std::endl; //debug

    }
    else if (request.getMethod() == "ERROR")
    {
        std::cout << "ERROR "<< std::endl; // debug

    }
}
