#include "../../includes/parsing.hpp"

void	parse_listen(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(void)config;
	(void)locconfig;
	(*index)++;

	if ((*tokens)[*index + 1] != ";" || (*tokens)[*index] == ";")
		throw std::runtime_error("Syntax error in listen directive");

	for (size_t i = 0; i < (*tokens)[*index].size(); ++i)
	{
		if (!isdigit((*tokens)[*index][i]))
			throw std::runtime_error("Value error port must contain only digits");
	}

	int port = std::atoi((*tokens)[*index].c_str());
	if (port <= 0 || port > 65535)
		throw std::runtime_error("Value error port must be in range 1 - 65535");
	(*servconf).setListen((*tokens)[*index]);
	(*index) += 2;
}

void	parse_host(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(void)config;
	(void)locconfig;
	(*index)++;

	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
		throw std::runtime_error("Syntax error in host directive");
	
	if ((*tokens)[*index] == "localhost")
	{
		(*servconf).setHost((*tokens)[*index]);
		(*index)++;
		return ;
	}

	for (size_t i = 0; i < (*tokens)[*index].size(); ++i)
	{
		if (!isdigit((*tokens)[*index][i]) && (*tokens)[*index][i] != '.')
			throw std::runtime_error("Value error wrong format of ip adress");
	}

	std::stringstream ss((*tokens)[*index]);
	std::string	ip;
	int nb_section = 0;	
	while (getline(ss, ip, '.'))
	{
		if (ip.empty() || ip.size() > 3 || ip.size() < 1)
			throw std::runtime_error("Value error the ip adrress is invalid");
		if (std::atoi(ip.c_str()) > 255)
			throw std::runtime_error("Value error the ip adrress is invalid");
		nb_section++;
	}
	if (nb_section != 4 || (*tokens)[*index][(*tokens)[*index].size() - 1] == '.')
		throw std::runtime_error("Value error the ip adrress is invalid");
	(*index) += 2;
	return ;
}

// void	parse_server_name(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
// {

// }

// void	parse_max_body_size(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
// {

// }

// void	parse_error_page(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
// {
	
// }



// void	parse_location(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *srevconf)
// {
// 	while ((*tokens)[*index] != "}" && *index <= tokens->size())
// 	{
// 		if ()

// 		*index++;
// 	}
// 	return ;
// }

// il me reste les 3 parties a parser, mettre les elements par defaut dans le constructeur si besoin