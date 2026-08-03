#include "includes/webserv.hpp"
#include "includes/http/HttpRequest.hpp"
#include "includes/http/HttpResponse.hpp"
#include "includes/config/Config.hpp"

int main(int argc, char **argv)
{
	Config *config;

	if(argc > 2)
	{
		std::cerr << "Error: Too many arguments" << std::endl;
		return (1);
	}
	try 
	{
		if (argc == 2)
			config = lexer(argv[1])
		else
			config = (lexer("default_config.conf"));
	}
	catch (std::exception &e)
	{
		std::cerr <<  e.what() << std::endl;
		return (1);
	}
	if (ft_webserv(config))
		return (ft_free_config (config), 1);
	return (ft_free_config (config), 0);
}