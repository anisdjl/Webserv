#include "../../includes/config/Config.hpp"

Config::Config(){};

Config::~Config(){};

/* Server conf get */

std::string	Config::getListen() const
{
	return this->server._listen;
}

std::string	Config::getHost() const
{
	return this->server._host;
}

std::string	Config::getServerName() const
{
	return this->server._server_name;
}

std::string	Config::getRoot() const
{
	return this->server._root;
}

std::string	Config::getIndex() const
{
	return this->server._index;
}

std::string	Config::getClientBodySize() const
{
	return this->server._client_body_size;
}

std::string	Config::getErrorPage() const
{
	return this->server._error_page;
}

/* Location conf get */

std::string	Config::getAllowMethods() const
{
	return this->location._allow_methods;
}

std::string	Config::getAutoIndex() const
{
	return this->location._auto_index;
}

std::string	Config::getUploadTo() const
{
	return this->location._upload_to;
}

std::string	Config::getCgiPath() const
{
	return this->location._cgi_path;
}

std::string	Config::getCgiExtention() const
{
	return this->location._cgi_extension;
}

std::string	Config::getReturn() const
{
	return this->location._return;
}

