#include "../../includes/parsing.hpp"

void	parse_location(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	if ((*tokens)[*index][0] != '/')
		throw std::runtime_error("Syntax error invalid path");
	else
	{
		(*locconfig).setPath((*tokens)[*index]);
		(*index)++;
	}
	if ((*tokens)[*index] != "{")
		throw std::runtime_error("Syntax error missing '{'");
	(*index)++;

	while ((*tokens)[*index] != "}" && (*index) < (*tokens).size())
	{
		if ((*tokens)[*index] == "root")
		{
			parse_root(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "index")
		{
			parse_index(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "allow_methods")
		{
			parse_methods(config, tokens, index, locconfig, servconf);
			continue;
		}
		// if ((*tokens)[*index] == "autoindex")
		// {
		// 	parse_autoindex(config, tokens, index, locconfig, servconf);
		// 	continue;
		// }
		// if ((*tokens)[*index] == "upload_store")
		// {
		// 	parse_upload(config, tokens, index, locconfig, servconf);
		// 	continue;
		// }
		// if ((*tokens)[*index] == "return")
		// {
		// 	parse_return(config, tokens, index, locconfig, servconf);
		// 	continue;
		// }
		// if ((*tokens)[*index] == "cgi_pass")
		// {
		// 	parse_cgi(config, tokens, index, locconfig, servconf);
		// 	continue;
		// }
		throw std::runtime_error("Error: wrong configuration file format");
	}
}

void	parse_root(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	(void)config; (void)servconf;

	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
		throw std::runtime_error("Syntax error in client max body size directive");

	(*locconfig).setRoot((*tokens)[*index]);

	(*index) += 2;
}

void	parse_index(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	(void)config; (void)servconf;

	while ((*tokens)[*index] != ";")
	{
		if ((*index) == (*tokens).size() - 1)
			throw std::runtime_error("Syntax error ';' missing");
		
		(*locconfig).setIndex((*tokens)[*index]);
		std::cout << (*tokens)[*index] << std::endl;
		(*index)++;
	}
	(*index)++;
}

void	parse_methods(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	(void)config; (void)servconf;

	while ((*tokens)[*index] != ";")
	{
		if ((*index) == (*tokens).size() - 1)
			throw std::runtime_error("Syntax error ';' missing");
		
		if ((*tokens)[*index] != "GET" && (*tokens)[*index] != "POST" && (*tokens)[*index] != "DELETE")
			throw std::runtime_error("Syntax error wrong method directive must be GET, POST or DELETE");
		std::cout << (*tokens)[*index] << std::endl;
		(*locconfig).setMethods((*tokens)[*index]);
		(*index)++;
	}
	(*index)++;
}