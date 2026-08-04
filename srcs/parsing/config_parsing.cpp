#include "../../includes/parsing.hpp"

static void	space(std::string &line)
{
	std::string	copy;
	for (size_t i = 0; i < line.length(); i++)
	{
		if (line[i] == ';' || line[i] == '{' || line[i] == '}')
		{
			copy += ' ';
			copy += line[i];
			copy += ' ';
		}
		else
			copy += line[i];
	}
	line.clear();
	line = copy;
}

static void	check_tokens(std::string &line)
{
	unsigned long	pos = line.find('#');
	if (pos != std::string::npos)
		line.erase(pos, line.length());
	return ;
}

static bool	empty_line(std::string &line)
{
	return (line.find_first_not_of(" \t") == std::string::npos);
}

std::vector<std::string>	*lexe_config(std::string filename)
{
	std::ifstream				file(filename.c_str());
	std::string					line;
	std::vector<std::string>	*tokens = new std::vector<std::string>;

	if (!file.is_open())
		throw std::runtime_error("Error: could not open the configuration file");
	std::string token;
	while (getline(file, line))
	{
		space(line);
		check_tokens(line);
		std::stringstream ss(line);
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
	Config						*config = new Config();

	tokens = lexe_config(filename);
	fsm(config, tokens);
}

void	parse_server(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	if ((*tokens)[*index] != "{")
		throw std::runtime_error("Error: wrong configuration file format");
	(*index)++;
	config->increment();

	while ((*tokens)[*index] != "}" && *index < (*tokens).size()) { // on check le } pcq celui de la location sera mange dans location
		if ((*tokens)[*index] == "location")
		{
			parse_location(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "listen")
		{
			parse_listen(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "host")
		{
			parse_host(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "server_name")
		{
			parse_server_name(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "client_max_body_size")
		{
			parse_max_body_size(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "error_page")
		{
			parse_error_page(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "}")
		{
			config->decrement();
			(*index)++;
			return ;	
		}
		throw std::runtime_error("Error: wrong configuration file format");
	}
}

void	fsm(Config *config, std::vector<std::string> *tokens)
{
	config->setState(MAIN_SECTION);

	ServerConfig					*serverconf = new ServerConfig;
	LocationConfig					*locationconf = new LocationConfig;

	size_t	index = 0;
	std::cout << "taille du vecteur de tokens " << tokens->size() << std::endl;
	while (index < tokens->size())
	{
		if ((*tokens)[index] != "server")
			throw std::runtime_error ("Error: wrong configuration file format");
		index++;
		parse_server(config, tokens, &index, locationconf, serverconf);
	}
	std::cout << "valeur de l'index a la fin du parsing" << std::endl;
	// delete tokens;
	// delete config; just for the test
}

// je dois trouver un moyen de checker si les {} sont bien ferme ou pas si le premier server a ete ferme ou pas
// donc je pense qu'il faut calculer combien on ete ouvert et fermer depuis avec un compteur genre ouver ++ et fermee -- et si c'est a 0 c'est que c'est bon 