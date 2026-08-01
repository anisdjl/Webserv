#include "../../includes/parsing.hpp"

LocationConfig::LocationConfig(void)
{
	_autoindex = false;
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

void	LocationConfig::setPath(std::string &path) {
	_path = path;
}

void	LocationConfig::setRoot(std::string &root) {
	_root = root;
}

void	LocationConfig::setIndex(std::string &index) {
	_index.push_back(index);
}

void	LocationConfig::setMethods(std::string &method) {
	_methods.push_back(method);
}