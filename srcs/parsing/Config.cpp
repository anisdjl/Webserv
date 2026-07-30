#include "../../includes/parsing.hpp"

Config::Config(void)
{
	_state = MAIN_SECTION;
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

void	Config::setState(STATE state)
{
	_state = state;
}

void	Config::increment(void)
{
	_nb_brackets++;
}

void	Config::decrement(void)
{
	_nb_brackets--;
}