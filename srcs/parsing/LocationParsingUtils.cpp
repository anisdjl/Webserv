#include "../../includes/config/Config.hpp"

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
	(*config).increment();
	while ((*index) < (*tokens).size()) // je viens de retirer la condition de while tokens != }
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
		if ((*tokens)[*index] == "autoindex")
		{
			parse_autoindex(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "upload_store")
		{
			parse_upload(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "return")
		{
			parse_return(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "cgi_pass")
		{
			parse_cgi(config, tokens, index, locconfig, servconf);
			continue;
		}
		if ((*tokens)[*index] == "}")
		{
			(*index)++;
			(*config).decrement();
			(*servconf).setLocations(locconfig);
			return ;
		}
		throw std::runtime_error("Error: wrong configuration file format 4");
	}
}

void	parse_root(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	(void)config; (void)servconf;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
		throw std::runtime_error("Syntax error incomplete configuration");

	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
		throw std::runtime_error("Syntax error in client max body size directive");

	(*locconfig).setRoot((*tokens)[*index]);

	(*index) += 2;
}

void	parse_index(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	(void)config; (void)servconf;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
		throw std::runtime_error("Syntax error incomplete configuration");
	
	while ((*tokens)[*index] != ";")
	{
		if ((*index) >= (*tokens).size())
			throw std::runtime_error("Syntax error incomplete configuration");

		if ((*index) == (*tokens).size() - 1)
			throw std::runtime_error("Syntax error ';' missing");
		
		(*locconfig).setIndex((*tokens)[*index]);
		(*index)++;
	}
	(*index)++;
}

void	parse_methods(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	(void)config; (void)servconf;

	locconfig->clearMethods();
	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
		throw std::runtime_error("Syntax error incomplete configuration");

	while ((*tokens)[*index] != ";")
	{
		if ((*index) == (*tokens).size() - 1)
			throw std::runtime_error("Syntax error ';' missing");
		
		if ((*index) >= (*tokens).size())
			throw std::runtime_error("Syntax error incomplete configuration");
		
		if ((*tokens)[*index] != "GET" && (*tokens)[*index] != "POST" && (*tokens)[*index] != "DELETE")
			throw std::runtime_error("Syntax error wrong method directive must be GET, POST or DELETE");

		(*locconfig).setMethods((*tokens)[*index]);
		(*index)++;
	}
	(*index)++;
}

void	parse_autoindex(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	(void)config; (void)servconf;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
		throw std::runtime_error("Syntax error incomplete configuration");

	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
		throw std::runtime_error("Syntax error in autindex directive");
	
	if ((*tokens)[*index] != "on" && (*tokens)[*index] != "off")
		throw std::runtime_error("Value error autoindex value must be 'on' or 'off'");

	(*locconfig).setAutoIndex((*tokens)[*index]);
	(*index) += 2;
}

void	parse_upload(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf)
{
	(*index)++;
	(void)config;
	(void)servconf;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
		throw std::runtime_error("Syntax error incomplete configuration");

	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] != ";")
		throw std::runtime_error("Syntax error in upload_store directive");
	
	(*locconfig).setUpload((*tokens)[*index]);
	(*index) += 2;
}

void	parse_return(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconf, ServerConfig *servconf)
{
	(*index)++;
	(void)config; (void)servconf;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
		throw std::runtime_error("Syntax error incomplete configuration");

	if ((*tokens)[*index] != ";" && (*tokens)[*index + 1] == ";")
	{
		for (size_t y = 0; y < (*tokens)[*index].size(); ++y)
			if (!isdigit((*tokens)[*index][y]))
				throw std::runtime_error("Value error the error code in the return directive must contain only digit");
		int code = std::atoi((*tokens)[*index].c_str());
		if (code > 599 || code < 100)
			throw std::runtime_error("Value error the error code in the return value must be in range [100 - 599]");
		(*locconf).setReturn(code);
		(*index) += 2;
		return ;
	}
	else if ((*tokens)[*index] != ";" && (*tokens)[*index + 1] != ";" && (*tokens)[*index + 2] == ";")
	{
		for (size_t y = 0; y < (*tokens)[*index].size(); ++y)
			if (!isdigit((*tokens)[*index][y]))
				throw std::runtime_error("Value error the error code in the return directive must contain only digit");
		int code = std::atoi((*tokens)[*index].c_str());
		if (code > 599 || code < 100)
			throw std::runtime_error("Value error the error code in the return value must be in range [100 - 599]");
		(*locconf).setReturn(code, (*tokens)[*index + 1]);
		(*index) += 3;
	}
	else
		throw std::runtime_error("Syntax error in return directive");
}

void	parse_cgi(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconf, ServerConfig *servconf)
{
	(*index)++;	
	(void)config; (void)servconf;

	if ((*index) >= (*tokens).size() || (*index + 1) >= (*tokens).size())
		throw std::runtime_error("Syntax error incomplete configuration");

	if ((*tokens)[*index] == ";" || (*tokens)[*index + 1] == ";" || (*tokens)[*index + 2] != ";")
		throw std::runtime_error("Syntax error in cgi directive");
	
	if ((*tokens)[*index][0] != '.')
		throw std::runtime_error("Syntax error the extension name in the cgi direvtive must begin with a '.'");
	(*locconf).setCgis((*tokens)[*index], (*tokens)[*index + 1]);
	(*index) += 3;
}