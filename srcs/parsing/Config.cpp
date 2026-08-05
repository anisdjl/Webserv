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

Config	&Config::operator=(const Config &src)
{
	if (&src != this)
	{
		std::cout << "je ferai ca plus tard" << std::endl;
	}
	return (*this);
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
		it->displayServConf();
	}
}