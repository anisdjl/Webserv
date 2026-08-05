#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../Webserv.hpp"

class Config
{
	private:
		std::vector<ServerConfig>			_servers; // le nombre de socket c'est le nombre de server config
		int									_nb_brackets;
	public:
		Config(void);
		Config(const Config &src);
		Config	&operator=(const Config &src);
		~Config(void);
		
		void	setServer(ServerConfig *servconf);
		void	displayConfig(void);
		void	increment(void) { _nb_brackets++; };
		void	decrement(void) { _nb_brackets--; };
		int		getNbBrackets(void) {return _nb_brackets;};

		std::vector<ServerConfig>	getServer(void) { return _servers; };
};

class LocationConfig
{
	private:
		std::string							_path;
		std::string							_root; // inherites from the server if not mentionned in the conf or error 404
		std::vector<std::string>			_index; // i don't remember
		std::vector<std::string>			_methods; // only GET if absent in the conf file
		bool								_autoindex; // off by default
		std::map<std::string, std::string>	_cgis; // cgis not allowed if not in the conf file
		std::string							_upload_store; // upload interdit par defaut si pas dans le fichier de conf
		std::map<int, std::string>			_return; // not mandatory, no redirection by default
	
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
		void	setCgis(std::string &extension, std::string &path);
		void	setUpload(std::string &upload);
		void	setReturn(int &code, std::string &path); // je mets void pour le moment mais c'est pas bon
		void	setReturn(int code);

		std::string							getPath(void) { return _path; };
		std::string							getRoot(void) { return _root; };
		std::vector<std::string>			getIndex(void) { return _index; };
		std::vector<std::string>			getMethods(void) { return _methods; };
		bool								getAutoindex(void) { return _autoindex; };
		std::map<std::string, std::string>	getCgis(void) { return _cgis; };
		std::string							getUploadStore(void) { return _upload_store; };
		std::map<int, std::string>			getReturn(void) { return _return; };

		void	clearMethods(void) { _methods.clear(); };
		void	displayLocation(void);
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

		void	setLocations(LocationConfig *locconfig);
		void	setListen(std::string &listen);
		void	setHost(std::string &host);
		void	setServerName(std::string &server_name);
		void	setClientMaxBody(long value);
		void	setErrorpage(int code, std::string &path); // je mets void pour le moment mais c'est pas bon

		std::string					getListen(void) { return _listen; };
		std::string					getHost(void) { return _host; };
		std::vector<std::string>	getServerName(void) { return _server_name; };
		long						getClientMaxBodySize(void)	{ return _client_max_body_size; };
		std::map<int, std::string>	getErrorPage(void) { return _error_page; };
		std::vector<LocationConfig>	getLocations(void) { return _locations; };

		void	displayServConf(void);
		LocationConfig	*matchLocation(const std::string& path);
};

std::vector<std::string>	*lexe_config(std::string filename);
Config						*lexer(std::string filename);
void						fsm(Config *config, std::vector<std::string> *tokens);

void						parse_listen(Config *config,std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_host(Config *config,std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_server_name(Config *config,std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_max_body_size(Config *config,std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_error_page(Config *config,std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);


void						parse_location(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *srevconf);
void						parse_root(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_index(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_methods(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_autoindex(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);;
void						parse_upload(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_return(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconf, ServerConfig *servconf);
void						parse_cgi(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconf, ServerConfig *servconf);
void						CheckConfig(Config &config);
void						CheckServer(ServerConfig &server);

#endif
