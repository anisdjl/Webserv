#include "../../includes/config/Config.hpp"

Config::Config() {}

Config::~Config() {}

void Config::setState(STATE state)
{
    this->_state = state;
}

// ServerConfig

ServerConfig::ServerConfig() {}

ServerConfig::~ServerConfig() {}

std::string ServerConfig::getListen() const
{
    return this->_listen;
}

std::string ServerConfig::getHost() const
{
    return this->_host;
}

const std::vector<std::string>& ServerConfig::getServerName() const
{
    return this->_server_name;
}

long ServerConfig::getClientMaxBodySize() const
{
    return this->_client_max_body_size;
}

const std::map<int, std::string>& ServerConfig::getErrorPage() const
{
    return this->_error_page;
}

const std::vector<LocationConfig>& ServerConfig::getLocations() const
{
    return this->_locations;
}

// LocationConfig

LocationConfig::LocationConfig() {}

LocationConfig::~LocationConfig() {}

std::string LocationConfig::getPath() const
{
    return this->_path;
}

std::string LocationConfig::getRoot() const
{
    return this->_root;
}

const std::vector<std::string>& LocationConfig::getIndex() const
{
    return this->_index;
}

const std::vector<std::string>& LocationConfig::getMethods() const
{
    return this->_methods;
}

bool LocationConfig::getAutoIndex() const
{
    return this->_autoindex;
}

const std::vector<std::string>& LocationConfig::getCgis() const
{
    return this->_cgis;
}

std::string LocationConfig::getUploadStore() const
{
    return this->_upload_store;
}

// func

LocationConfig*   ServerConfig::matchLocation(const std::string& path)
{
	LocationConfig	*match = NULL;
	int				match_len = 0;
	for (size_t i = 0; i < _locations.size(); i++)
	{
		std::string location_url = _locations[i].getPath();
		if (path.compare(0, location_url.size(), location_url) == 0)
		{
			if (location_url.size() > match_len) // nv fav trouver
			{
				match = &_locations[i];
				match_len = location_url.size();
			}
		}
	}
	return (match);
}

/*
    if ((str1.compare(3, 5, str2)) == 0)
        cout << "Substring Matched";
    else
        cout << "Strings Not Matched";
*/