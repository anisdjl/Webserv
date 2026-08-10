#include "../../includes/http/HttpRequest.hpp"


#include <iostream>
#include <string>

void ft

bool ft_parse_first_line(HttpRequest &request)
{
    std::string buffer  = request.getBuffer();

    if (HttpRequest.)
}


bool ft_parse_http_request(HttpRequest &request, char *buffer)
{
    request.addToBuffer(std::string(buffer));
    if (request.getAvancement() == NOT_STARTED &&  ft_parse_first_line(request))
        return true;
    else if (request.getAvancement() == FIRST_LINE && ft_parse_headers(request)) 
        return true;
    else if (request.getAvancement() == HEADER && ft_parse_body(request))
        return true;
    return false;
}
