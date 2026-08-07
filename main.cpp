# include "includes/webserv.hpp"


int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Error: wrong number of arguments, try with: ./webserv [configuration file] or ./Webserv" << std::endl;
		return (1);
	}
	// Connection *socket = new Connection;
	Config *config = new Config;
	config = lexer(argv[1]);
	(void)config;
	const char *buffer = 
    "POST /api/user HTTP/1.1\r\n"
    "Host: localhost:8080\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: 18\r\n"
    "\r\n"
    "{\"name\": \"djelili\"}";
	ssize_t							bytes = std::strlen(buffer);
	try
	{
		//ft_parse_request(*socket, config->getServer()[(*socket).getServerIndex()], buffer, bytes);
		parserequests(buffer, bytes);
		// if (argc == 2)
		// 	lexer(argv[1]);
		// else
		// 	lexer("default_config.conf");
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}