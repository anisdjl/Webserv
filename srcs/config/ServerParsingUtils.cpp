#include "../../includes/config/Config.hpp"

void	parse_listen(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	(void)config;
	(*index)++;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error incomplete configuration");
	}
	if ((*tokens)[*index + 1] != ";" || (*tokens)[*index] == ";")
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error in listen directive");
	}
	for (size_t i = 0; i < (*tokens)[*index].size(); ++i)
		if (!isdigit((*tokens)[*index][i]))
		{
			delete tokens;
			delete_all(config);
			delete servconf;
			throw std::runtime_error("Value error port must contain only digits");
		}
	int port = std::atoi((*tokens)[*index].c_str());
	if (port <= 0 || port > 65535)
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Value error port must be in range 1 - 65535");
	}
	(*servconf).setListen((*tokens)[*index]);
	(*index) += 2;
}

void	parse_host(Config *config,std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	(void)config;
	(*index)++;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error incomplete configuration");
	}
	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error in host directive");
	}
	if ((*tokens)[*index] == "localhost")
	{
		(*servconf).setHost((*tokens)[*index]);
		(*index)++;
		return ;
	}

	for (size_t i = 0; i < (*tokens)[*index].size(); ++i)
		if (!isdigit((*tokens)[*index][i]) && (*tokens)[*index][i] != '.')
		{
			delete tokens;
			delete_all(config);
			delete servconf;
			throw std::runtime_error("Value error wrong format of ip adress");
		}
	std::stringstream ss((*tokens)[*index]);
	std::string	ip;
	int nb_section = 0;	
	while (getline(ss, ip, '.'))
	{
		if (ip.empty() || ip.size() > 3 || ip.size() < 1)
		{
			delete tokens;
			delete_all(config);
			delete servconf;
			throw std::runtime_error("Value error the ip adrress is invalid");
		}
		if (std::atoi(ip.c_str()) > 255)
		{
			delete tokens;
			delete_all(config);
			delete servconf;
			throw std::runtime_error("Value error the ip adrress is invalid");
		}
		nb_section++;
	}
	if (nb_section != 4 || (*tokens)[*index][(*tokens)[*index].size() - 1] == '.')
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Value error the ip adrress is invalid");
	}
	(*servconf).setHost((*tokens)[*index]);
	(*index) += 2;
}

void	parse_server_name(Config *config,std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	(void)config;
	(*index)++;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error incomplete configuration");
	}
	if ((*tokens)[*index] == ";")
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error the server name can't be empty");
	}
	while ((*tokens)[*index] != ";")
	{
		if ((*index) == (*tokens).size() - 1)
		{
			delete tokens;
			delete_all(config);
			delete servconf;
			throw std::runtime_error("Syntax error ';' missing");
		}
		if ((*index) >= (*tokens).size())
		{
			delete tokens;
			delete_all(config);
			delete servconf;
			throw std::runtime_error("Syntax error incomplete configuration");
		}
		(*servconf).setServerName((*tokens)[*index]);
		(*index)++;
	}
	(*index)++;
}

void	parse_max_body_size(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	(void)config;
	(*index)++;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error incomplete configuration");
	}
	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
	{
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error in client max body size directive");
	}
	for (size_t y = 0; y < (*tokens)[*index].size(); ++y)
		if (!isdigit((*tokens)[*index][y]))
			{
				delete tokens;
				delete_all(config);
				delete servconf;
				throw std::runtime_error("Value error the client max body size must contain only digits");
			}
	long value = std::atoi((*tokens)[*index].c_str());
	if (value < 0)
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Value error the client_max_body_size must be positive");
	}
	(*servconf).setClientMaxBody(value);
	(*index) += 2;
}

void	parse_error_page(Config *config,std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	(void)config;

	(*index)++;
	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error incomplete configuration");
	}
	std::vector<int>	codes;
	while ((*tokens)[*index + 1] != ";" && (*index) + 1 != (*tokens).size() - 1)
	{
		if ((*index) >= (*tokens).size())
		{
			delete tokens;
			delete_all(config);
			delete servconf;
			throw std::runtime_error("Syntax error incomplete configuration");
		}
		for (size_t y = 0; y < (*tokens)[*index].size(); ++y)
			if (!isdigit((*tokens)[*index][y]))
			{
				delete tokens;
				delete_all(config);
				delete servconf;
				throw std::runtime_error("Value error invalid error code");
			}
		int code = std::atoi((*tokens)[*index].c_str());
		if (code < 300 || code > 599)
		{
			delete tokens;
			delete_all(config);
			delete servconf;
			throw std::runtime_error("Value error error code value must be between 300 - 599");
		}
		codes.push_back(code);
		(*index)++;
	}
	std::string path = (*tokens)[*index];
	for (size_t y = 0; y < codes.size(); ++y)
		(*servconf).setErrorpage(codes[y], path);
	(*index) += 2;
}


void	parse_autoindex_server(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	(*index)++;
	(void)config;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error incomplete configuration");
	}
	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error in autindex directive");
	}
	if ((*tokens)[*index] != "on" && (*tokens)[*index] != "off")
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Value error autoindex value must be 'on' or 'off'");
	}
	(*servconf).setAutoIndexfound(true);
	(*servconf).setAutoindex((*tokens)[*index]);
	(*index) += 2;
}

void	parse_cookie_server(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf)
{
	(*index)++;
	(void)config;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error incomplete configuration");
	}
	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Syntax error in cookies directive");
	}
	if ((*tokens)[*index] != "on" && (*tokens)[*index] != "off")
	{
		delete tokens;
		delete_all(config);
		delete servconf;
		throw std::runtime_error("Value error cookies value must be 'on' or 'off'");
	}
	(*servconf).setCookies((*tokens)[*index]);
	(*index) += 2;
}


void	delete_all(Config *config)
{
	size_t	nb_servconf = config->getServer().size();

	std::vector<ServerConfig> server = config->getServer();

	for (size_t i = 0; i < nb_servconf; ++i)
	{
		std::vector<LocationConfig>	locations = server[i].getLocations();

		size_t nb_location = locations.size();
		for (size_t y = 0; y < nb_location; ++y)
			delete &locations[y];
		delete &server[i];
	}
	delete config;
}