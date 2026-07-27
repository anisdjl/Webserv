#include "../../includes/parsing.hpp"

// on va faire le parsing pour permettre par la suite l'acces facile a certaine info du serveur comme la ou on doit chercher les fichiers etc 

static void	check_tokens(std::string &line)
{
	unsigned long	pos = line.find('#');
	if (pos != std::string::npos)
		line.erase(pos, line.length() - pos);
	return ;
}

static bool	empty_line(std::string &line)
{
	return (line.find_first_not_of(" \t") == std::string::npos);
}

std::vector<std::string>	*parse_config(std::string filename)
{
	std::ifstream				file(filename.c_str());
	std::string					line;
	std::vector<std::string>	*tokens = new std::vector<std::string>;

	if (!file.is_open())
		throw std::runtime_error("Error: could not open the configuration file");
	std::string token;
	while (getline(file, line))
	{
		std::stringstream ss(line);
		check_tokens(line);		
		if (empty_line(line) == false)
		{
			while (ss >> token)
				(*tokens).push_back(token);
		}
	}
	file.close();
	return (tokens);
}

void	lexer(std::string filename)
{
	std::vector<std::string>	*tokens;

	tokens = parse_config(filename);
	for (std::vector<std::string>::iterator it = (*tokens).begin(); it < (*tokens).end(); ++it)
		std::cout << *it << std::endl;
}

// je recupere tout le flux, je garde tout les mots dans un vecteur, comme ca je n'aurais plus les espaces
// une fois le vecteur recuperer je peux faire ma machine a etat