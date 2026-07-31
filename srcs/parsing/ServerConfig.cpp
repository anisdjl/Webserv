#include "../../includes/parsing.hpp"

ServerConfig::ServerConfig(void)
{
	_listen = "NULL";
	_host = "NULL";
	// et rajouter tout le reste en fonction de ce qui est obligatoire ou pas
}

ServerConfig::~ServerConfig(void)
{

}

ServerConfig	&ServerConfig::operator=(const ServerConfig &src)
{
	(void)src;
	return (*this);
}

ServerConfig::ServerConfig(const ServerConfig &src)
{
	(void)src;
}

void	ServerConfig::setListen(std::string &listen)
{
	_listen = listen;
}

void	ServerConfig::setHost(std::string &host)
{
	_host = host;
}

