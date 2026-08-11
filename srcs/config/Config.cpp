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

Config::~Config(void)
{
	// vide pour le moment mais on va free apres
}

std::map<int, std::string >::const_iterator ServerConfig::findErrorPage(int key) const
{
    return (_error_page.find(key));
}

// func

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
