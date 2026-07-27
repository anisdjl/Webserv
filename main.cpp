#include "includes/webserv.hpp"
#include "includes/http/HttpRequest.hpp"

int main()
{
    HttpRequest req;

    req.setMethod("POST");
    req.setPath("/cgi-bin/upload.py");
    req.setQueryString("user=42&action=save");
    req.setVersion("HTTP/1.1");
    
    req.setHeader("host", "localhost:8080");
    req.setHeader("user-agent", "curl/7.81.0");
    req.setHeader("accept", "*/*");
    req.setHeader("content-type", "application/x-www-form-urlencoded");
    req.setHeader("content-length", "27");
    req.setHeader("connection", "close");
    
    req.setBody("name=JohnDoe&age=25&status=ok");
    req.setError(0);
    return 0;
}