#include "../../includes/parsing.hpp"

// on va faire le parsing pour permettre par la suite l'acces facile a certaine info du serveur comme la ou on doit chercher les fichiers etc 

void	parse_config(std::string filename)
{
	std::ifstream	file(filename.c_str());
	std::string		line;

	while (getline(file, line))
		std::cout << line << std::endl;
}
