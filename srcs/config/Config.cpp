#include "../../includes/config/Config.hpp"

Config::Config(void)
{
	_nb_brackets = 0;
}

Config::Config(const Config &src)
{
	(void)src;
	// on laisse vide pour le moment
}

std::map<int, std::string >::const_iterator ServerConfig::findErrorPage(int key) const
{
    return (_error_page.find(key));
}


// LocationConfig

LocationConfig::LocationConfig() {}

LocationConfig::~LocationConfig() {}

long		LocationConfig::getClientMaxBodySize() const
{
	return this->_client_max_body_size;
}

// func

LocationConfig*   ServerConfig::matchLocation(const std::string& path)
{
	LocationConfig	*match = NULL;
	size_t			match_len = 0;
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

Config::~Config(void)
{
	// vide pour le moment mais on va free apres
}


void	Config::setServer(ServerConfig *servconf)
{
	_servers.push_back(*servconf);
	//(*servconf).displayServConf();
	delete servconf;
}

void	Config::displayConfig(void)
{
	std::cout << "number of server config " << this->_servers.size() << std::endl;
	for (std::vector<ServerConfig>::iterator it = this->_servers.begin(); it != this->_servers.end(); ++it)
	{
		std::cout << "server config :" <<std::endl;
		// it->displayServConf();
	}
}
