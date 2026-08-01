#include "../../includes/parsing.hpp"

LocationConfig::LocationConfig(void)
{

}

LocationConfig::~LocationConfig(void)
{

}

LocationConfig	&LocationConfig::operator=(const LocationConfig &src)
{
	(void)src;

	return (*this);
}

LocationConfig::LocationConfig(const LocationConfig &src)
{
	(void)src;
}

void	LocationConfig::setPath(std::string &path)
{
	_path = path;
}