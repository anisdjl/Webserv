#include "../../includes/parsing.hpp"

ServerConfig::ServerConfig(void)
{
	_listen = "NULL";
	_host = "NULL";
	_client_max_body_size = 1000000;
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

void	ServerConfig::setClientMaxBody(long value)
{
	_client_max_body_size = value;
}

void	ServerConfig::setServerName(std::string &server_name)
{
	_server_name.push_back(server_name);
}

void	ServerConfig::setErrorpage(int code, std::string &path)
{
	_error_page.insert(std::make_pair(code, path));
}

void	ServerConfig::setLocations(LocationConfig *locconfig)
{
	_locations.push_back(*locconfig);
	// delete (locconfig); je suis pas sur de la delete ici
}