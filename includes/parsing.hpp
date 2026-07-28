#ifndef PARSING_HPP
# define PARSING_HPP

# include "webserv.hpp"

enum STATE {
	SERVER_SECTION,
	LOCATION_SECTION,
	MAIN_SECTION
};

class Request
{
	// un etat is_complete pour savoir si la requete est complete ou pas

	private:
		std::string							_start_line;
		std::map<std::string, std::string>	_header;
		std::string							_body;
		long								_content_length;

	public:
		Request(void);
		Request(const Request &src);
		Request &operator=(const Request &src);
		~Request(void);
};


class Config
{
	private:
		STATE								_state;

	public:
		Config(void);
		Config(const Config &src);
		Config	&operator=(const Config &src);
		~Config(void);
		void	setState(STATE state);
};

class LocationConfig
{
	private:
		

	public:
		LocationConfig(void);
		LocationConfig(const LocationConfig &src);
		~LocationConfig(void);
		LocationConfig	&operator=(const LocationConfig &src);
};
	
std::vector<std::string>	*lexe_config(std::string filename);
void						lexer(std::string filename);
void						fsm(Config *config, std::vector<std::string> *tokens);

#endif