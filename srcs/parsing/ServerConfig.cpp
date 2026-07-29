#include "../../includes/parsing.hpp"

ServerConfig::ServerConfig(void)
{

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