#include "../../includes/config/Config.hpp"

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

Config	*lexer(std::string filename)
{
	std::vector<std::string>	*tokens;
	Config						*config = new Config();

	tokens = lexe_config(filename);
	fsm(config, tokens);
	return (config);
}

void	parse_server(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	if ((*tokens)[*index] != "{")
		throw std::runtime_error("Error: wrong configuration file format 2");
	(*index)++;
	(*config).increment();
	while (*index < (*tokens).size()) // je viens de retirer le condition du while (tokens != })
	{
		if ((*tokens)[*index] == "root")
		{
			parse_root_server(config, tokens, index, servconf);
			continue;
		}
		if ((*tokens)[*index] == "location")
		{
			LocationConfig					*locconfig = new LocationConfig;
			parse_location(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "listen")
		{
			parse_listen(config, tokens, index, servconf);
			continue;
		}
		if ((*tokens)[*index] == "host")
		{
			parse_host(config, tokens, index, servconf);
			continue;
		}
		if ((*tokens)[*index] == "server_name")
		{
			parse_server_name(config, tokens, index, servconf);
			continue;
		}
		if ((*tokens)[*index] == "client_max_body_size")
		{
			parse_max_body_size(config, tokens, index, servconf);
			continue;
		}
		if ((*tokens)[*index] == "error_page")
		{
			parse_error_page(config, tokens, index, servconf);
			continue;
		}
		if ((*tokens)[*index] == "autoindex")
		{
			parse_autoindex_server(config, tokens, index, servconf);
			continue;
		}
		if ((*tokens)[*index] == "}")
		{
			// si on est ici c'est qu'on a fini le server actuel
			(*index)++;
			(*config).decrement();
			(*config).setServer(servconf);
			return ;	
		}
		throw std::runtime_error("Error: wrong configuration file format 3");
	}
}

void	parse_root_server(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	(*index)++;
	(void)config;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
		throw std::runtime_error("Syntax error incomplete configuration");

	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
		throw std::runtime_error("Syntax error in client max body size directive");

	(*servconf).setRoot((*tokens)[*index]);

	(*index) += 2;
}

void	fsm(Config *config, std::vector<std::string> *tokens)
{
	size_t	index = 0;
	while (index < tokens->size())
	{
		if ((*tokens)[index] != "server")
		{
			std::cout << "index actuel " << index << " token actuel " << (*tokens)[index] << " token d'avant " << (index > 0 ? (*tokens)[index -1] : std::string("none")) << " token d'apres " << (index + 1 < (*tokens).size() ? (*tokens)[index + 1] : std::string("none")) << std::endl;	
			throw std::runtime_error ("Error: wrong configuration file format 1");
		}
		ServerConfig					*serverconf = new ServerConfig;
		index++;
		parse_server(config, tokens, &index, serverconf);
	}
	if ((*config).getNbBrackets() != 0)
		throw std::runtime_error("Syntax error missung brackets");
	CheckConfig(*config);
	// (*config).displayConfig();
}

// je dois trouver un moyen de checker si les {} sont bien ferme ou pas si le premier server a ete ferme ou pas
// donc je pense qu'il faut calculer combien on ete ouvert et fermer depuis avec un compteur genre ouver ++ et fermee -- et si c'est a 0 c'est que c'est bon 