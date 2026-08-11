#include "../../includes/parsing.hpp"

ServerConfig::ServerConfig(void)
{
	_listen = "80";
	_host = "0.0.0.0";
	_client_max_body_size = 1000000;
	_autoindexfound = false;
	_root = "";
}

ServerConfig::~ServerConfig(void)
{

}

ServerConfig	&ServerConfig::operator=(const ServerConfig &src)
{
	if (this != &src)
	{
		_listen = src._listen;
		_host = src._host;
		_server_name = src._server_name;
		_client_max_body_size = src._client_max_body_size;
		_error_page = src._error_page;
		_locations = src._locations;
	}
	return (*this);
}

ServerConfig::ServerConfig(const ServerConfig &src)
{
	(*this) = src;
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
	//(*locconfig).displayLocation();
	delete (locconfig);
}

void	ServerConfig::displayServConf(void)
{
	std::cout << "=== server config ===\n" << 
	"listen: " << (*this)._listen << 
	" host: " << (*this)._host << std::endl;

	std::cout << "server name : ";
	for (std::vector<std::string>::iterator it = (*this)._server_name.begin(); it != (*this)._server_name.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << std::endl;
	
	std::cout << "client max body size " << (*this)._client_max_body_size << std::endl;
	
	std::cout << "error pages :";
	for (std::map<int, std::string>::iterator it = (*this)._error_page.begin(); it != (*this)._error_page.end(); ++it)
		std::cout << "code " << it->first << " error path " << it->second << std::endl;
	std::cout << std::endl;
	
	std::cout << "nb of location " << (*this)._locations.size() << std::endl; 
}

void	ServerConfig::setAutoindex(std::string &autoindex)
{
	if (autoindex == "on")
		_autoindex = true;
	else if (autoindex == "off")
		_autoindex = false;
}

void	ServerConfig::setRoot(std::string &root)
{
	_root = root;
}

void	ServerConfig::setAutoIndexfound(bool found)
{
	_autoindexfound = found;
}