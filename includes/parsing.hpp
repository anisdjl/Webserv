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
		//STATE								_state;

	public:
		Config(void);
		Config(const Config &src);
		Config	&operator=(const Config &src);
		~Config(void);
	};
	
std::vector<std::string>	*parse_config(std::string filename);
void						lexer(std::string filename);

#endif