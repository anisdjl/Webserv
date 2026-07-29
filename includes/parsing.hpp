#ifndef PARSING_HPP
# define PARSING_HPP

# include "webserv.hpp"

class LocationConfig;
class ServerConfig;

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
		std::vector<ServerConfig>			_servers; // le nombre de socket c'est le nombre de server config

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
		std::string					_root; // inherites from the server if not mentionned in the conf or error 404
		std::vector<std::string>	_index; // i don't remember
		std::vector<std::string>	_methods; // only GET if absent in the conf file
		bool						_autoindex; // off by default
		std::vector<std::string>	_cgis; // cgis not allowed if not in the conf file
		std::string					_upload_store; // upload interdit par defaut si pas dans le fichier de conf
		std::map<int, std::string>	_return; // not mandatory, no redirection by default
	
	public:
		LocationConfig(void);
		LocationConfig(const LocationConfig &src);
		~LocationConfig(void);
		LocationConfig	&operator=(const LocationConfig &src);
};

class	ServerConfig
{
	private:
		std::string					_listen;
		std::string					_host;
		std::vector<std::string>	_server_name;
		long						_client_max_body_size;
		std::map<int, std::string>	_error_page;
		std::vector<LocationConfig>	_locations;

	public:
		ServerConfig(void);
		~ServerConfig(void);
		ServerConfig	&operator=(const ServerConfig &src);
		ServerConfig(const ServerConfig &src);
};

std::vector<std::string>	*lexe_config(std::string filename);
void						lexer(std::string filename);
void						fsm(Config *config, std::vector<std::string> *tokens);

#endif