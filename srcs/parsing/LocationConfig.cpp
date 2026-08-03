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

void	LocationConfig::setAutoIndex(std::string &autoindex) {
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
}

void	LocationConfig::setUpload(std::string &upload)
{
	_upload_store = upload;
}

void	LocationConfig::setReturn(int &code, std::string &path)
{
	_return.insert(std::make_pair(code, path));
}
