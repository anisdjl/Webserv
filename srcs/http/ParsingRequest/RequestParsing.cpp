#include "../../includes/http/HttpRequest.hpp"

bool ft_parse_http_request(HttpRequest &request, char *buffer, int bytes_read)
{
    