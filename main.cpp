#include "includes/webserv.hpp"
#include "includes/http/HttpRequest.hpp"
#include "includes/http/HttpResponse.hpp"
#include "includes/config/Config.hpp"

int main()
{
    ServerConfig servConf;

    std::cout << "=== Test 1 : 400 Bad Request ===" << std::endl;
    HttpRequest req1;
    req1.setMethod("GET");
    req1.setPath("/");
    req1.setVersion("HTTP/1.1");
    req1.setError(400);

    HttpResponse resp1;
    std::string raw1 = resp1.buildResponse(req1, servConf);
    std::cout << raw1 << std::endl;

	std::cout << "=== Test 2 : 501 Not Implemented) ===" << std::endl;
    HttpRequest req2;
    req2.setMethod("PUT");
    req2.setPath("/index.html");
    req2.setVersion("HTTP/1.1");
    req2.setError(0);

    HttpResponse resp2;
    std::string raw2 = resp2.buildResponse(req2, servConf);
    std::cout << raw2 << std::endl;

	std::cout << "=== Test 3 : 404 not found) ===" << std::endl;
    HttpRequest req3;
    req3.setMethod("GET");
    req3.setPath("/inexistant.html");
    req3.setVersion("HTTP/1.1");
    req3.setError(404);

    HttpResponse resp3;
    std::string raw3 = resp3.buildResponse(req3, servConf);
    std::cout << raw3 << std::endl;

	// if(argc > 2)
	// {
	// 	std::cerr << "Error: Too many arguments" << std::endl;
	// 	return (1);
	// }
	// try 
	// {
	// 	if (argc == 2)
	// 		config = lexer(argv[1]);
	// 	else
	// 		config = (lexer("default_config.conf"));
	// }
	// catch (std::exception &e)
	// {
	// 	std::cerr <<  e.what() << std::endl;
	// 	return (1);
	// }
	// if (ft_webserv(config))
	// 	return (ft_free_config (config), 1);
	// return (ft_free_config (config), 0);
    return (0);
}