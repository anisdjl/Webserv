# include "includes/webserv.hpp"


int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Error: wrong number of arguments, try with: ./webserv [configuration file] or ./Webserv" << std::endl;
		return (1);
	}
	try
	{
		if (argc == 2)
			parse_config(argv[1]);
		else
			parse_config("default_config.conf");
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}