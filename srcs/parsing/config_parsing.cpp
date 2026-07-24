#include "../../includes/parsing.hpp"

// on va faire le parsing pour permettre par la suite l'acces facile a certaine info du serveur comme la ou on doit chercher les fichiers etc 

// static void	check_line(std::string &line)
// {
// 	std::cout << line << std::endl;
// }

void	parse_config(std::string filename)
{
	std::ifstream				file(filename.c_str());
	std::string					line;
	// std::vector<std::string>	tokens;

	if (!file.is_open())
		throw std::runtime_error("Error: could not open the configuration file");

	//Config *config = new Config();

	// config->_state = MAIN_SECTION;
	while (getline(file, line))
	{
		std::cout << line << std::endl;
	}
	file.close();
}


// je recupere tout le flux, je garde tout les mots dans un vecteur, comme ca je n'aurais plus les espaces
// une fois le vecteur recuperer je peux faire ma machine a etat