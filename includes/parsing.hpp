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
		int									_nb_brackets;

	public:
		void	increment(void);
		void	decrement(void);
		Config(void);
		Config(const Config &src);
		Config	&operator=(const Config &src);
		~Config(void);
		void	setState(STATE state);
};

class LocationConfig
{
	private:
		std::string					_path;
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

		void	setPath(std::string &path);
		void	setRoot(std::string &root);
		void	setIndex(std::string &index);
		void	setMethods(std::string &method);
		void	setAutoIndex(std::string &autoindex);
		void	setCgis(std::string &cgi);
		void	setUpload(std::string &upload);
		void	setReturn(int &code, std::string &path); // je mets void pour le moment mais c'est pas bon
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

		void	setListen(std::string &listen);
		void	setHost(std::string &host);
		void	setServerName(std::string &server_name);
		void	setClientMaxBody(long value);
		void	setErrorpage(int code, std::string &path); // je mets void pour le moment mais c'est pas bon
};

std::vector<std::string>	*lexe_config(std::string filename);
void						lexer(std::string filename);
void						fsm(Config *config, std::vector<std::string> *tokens);
void						parse_listen(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_host(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_server_name(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_max_body_size(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_error_page(Config *config,std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_location(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *srevconf);
void						parse_root(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_index(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_methods(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_autoindex(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);;
void						parse_upload(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_return(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconf, ServerConfig *servconf);

#endif