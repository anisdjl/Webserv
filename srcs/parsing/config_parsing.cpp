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

void	fsm(Config *config, std::vector<std::string> *tokens)
{
	config->setState(MAIN_SECTION);

	for (std::vector<std::string>::iterator it = (*tokens).begin(); it < (*tokens).end(); ++it)
	{
		if (*it != "server")
			throw std::runtime_error ("Error: wrong configuration file format");
		config->setState(SERVER_SECTION);
	}
	// delete tokens;
	// delete config; just for the test
}