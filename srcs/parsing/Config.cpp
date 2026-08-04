#include "../../includes/parsing.hpp"

Config::Config(void)
{
	
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
