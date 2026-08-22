#include "../../includes/config/Config.hpp"

LocationConfig::LocationConfig(void) : _path(""), _root(""), _autoindex(false), _upload_store(""), _autoindexfound(false) ,_cookies(false) 
{
	_methods.push_back("GET");
}

LocationConfig::~LocationConfig(void) {}

LocationConfig	&LocationConfig::operator=(const LocationConfig &src)
{
	if (this != &src)
	{
		_path = src._path;
		_root = src._root;
		_index = src._index;
		_methods = src._methods;
		_autoindex = src._autoindex;
		_cgis = src._cgis;
		_upload_store = src._upload_store;
		_autoindexfound = src._autoindexfound;
		_cookies = src._cookies;
		_return = src._return;
	}

	return (*this);
}

LocationConfig::LocationConfig(const LocationConfig &src)
{
	(*this) = src;
}

void	LocationConfig::setPath(const std::string &path) 
{
	_path = path;
}

void	LocationConfig::setRoot(const std::string &root) 
{
	_root = root;
}

void	LocationConfig::setIndex(const std::string &index) 
{
	_index.push_back(index);
}

void	LocationConfig::setMethods(const std::string &method) 
{
	_methods.push_back(method);
}

void	LocationConfig::setAutoIndex(std::string &autoindex) 
{
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
}

void	LocationConfig::setUpload(const std::string &upload)
{
	_upload_store = upload;
}

void	LocationConfig::setReturn(int code, const std::string &path)
{
	_return.insert(std::make_pair(code, path));
}

void	LocationConfig::setReturn(int code)
{
	_return.insert(std::make_pair(code, ""));
	// _return[code] = ""; ce cas sera en cas de doublon si on doit gerer les doublons pas sur de le garder
}

void	LocationConfig::setCgis(const std::string &extension,const std::string &path)
{
	_cgis.insert(std::make_pair(extension, path));
}

void	LocationConfig::displayLocation(void)
{
	std::cout << "=== location config ===" << std::endl;
	std::cout << "path " << (*this)._path << std::endl;
	std::cout << "root " << (*this)._root << std::endl;

	std::cout << "index ";
	for (std::vector<std::string>::iterator it = (*this)._index.begin(); it != (*this)._index.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << std::endl;

	std::cout << "methods ";
	for (std::vector<std::string>::iterator it = (*this)._methods.begin(); it != (*this)._methods.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << std::endl;


	std::cout << "autoindex " << (*this)._autoindex << std::endl;

	std::cout << "cgis ";
	for (std::map<std::string, std::string>::iterator it = (*this)._cgis.begin(); it != (*this)._cgis.end(); ++it)
		std::cout << "cgi extension " << it->first << " cgi path " << it->second << std::endl;
	std::cout << std::endl;

	std::cout << "upload stores " << (*this)._upload_store << std::endl;

	std::cout << "return : ";
	for (std::map<int, std::string>::iterator it = (*this)._return.begin(); it != (*this)._return.end(); ++it)
		std::cout << "code " << it->first << " return path " << it->second << std::endl;
	std::cout << std::endl;
}
