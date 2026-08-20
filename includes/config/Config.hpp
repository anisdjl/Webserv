#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../Webserv.hpp"

class LocationConfig;
class ServerConfig;
class HttpRequest;

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
        std::string                     		_path;
        std::string                     		_root;
        std::vector<std::string>        		_index;
        std::vector<std::string>        		_methods;
        bool                            		_autoindex;
        std::map<std::string, std::string>      _cgis;
        long                            		_client_max_body_size;
        std::string                     		_upload_store;
		std::map<int, std::string>				_return;
		bool									_autoindexfound;
		bool									_cookies;

    public:
        LocationConfig();
		LocationConfig(const LocationConfig &src);
        ~LocationConfig();
		LocationConfig &operator=(const LocationConfig &src);

		const std::string					getPath(void) const{ return _path; };
		const std::string					getRoot(void) const { return _root; };
		std::vector<std::string>			getIndex(void) const { return _index; };
		std::vector<std::string>			getMethods(void) const { return _methods; };
		bool								getAutoindex(void) const { return _autoindex; };
		bool								getAutoindexDefine(void) { return _autoindexfound; };
		std::map<std::string, std::string>	getCgis(void) const { return _cgis; };
		std::string							getUploadStore(void) { return _upload_store; };
		std::map<int, std::string>			getReturn(void) const { return _return; };
		long                            	getClientMaxBodySize(void) const { return _client_max_body_size; };
		bool								getCookies(void) {return this->_cookies;};

		void	setCookies(std::string found) { if (found == "on") _cookies = true; else _cookies = false; };
		void    setPath(const std::string &path);
        void    setRoot(const std::string &root);
        void    setIndex(const std::string &index);
        void    setMethods(const std::string &method);
        void    setAutoIndex(std::string &autoindex);
        void    setUpload(const std::string &upload);
        void    setReturn(int code, const std::string &path);
        void    setReturn(int code);
        void    setCgis(const std::string &extension, const std::string &path);
		void	setAutoIndexfound(bool found);
		
		void	clearMethods(void) { _methods.clear(); };
		void	displayLocation(void);
		// === debug ===
        void addMethod(std::string method) { _methods.push_back(method); }
};

class	ServerConfig
{
    private:
		std::string                     _root;
        std::vector<std::string>        _listen;
        std::string                     _host;
        std::vector<std::string>        _index; // NEED TO IMPLEMENT IT FOR EVAN
		bool							_autoindex;
        std::vector<std::string>        _server_name;
        std::string                     _upload_store; // NEED IT FOR EVAN
        long                            _client_max_body_size;
        std::map<int, std::string>      _error_page;
        std::vector<LocationConfig>     _locations;
		bool							_autoindexfound;
		bool							_listen_init;
		bool							_cookies;

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
		void	setAutoindex(std::string &autoindex); // need it for evan
		void	setErrorpage(int code, std::string &path); // je mets void pour le moment mais c'est pas bon//il faut set a 400 le code si faux
		void	setRoot(std::string &root);
		void	setAutoIndexfound(bool found);
		void	setCookies(std::string found) { if (found == "on") _cookies = true; else _cookies = false; };

		
		std::vector<std::string>		&getListen(void) { return _listen; };
		std::string						getHost(void) { return _host; };
		std::string						getRoot(void) { return _root; };
		std::string						getUploadStore(void) { return _upload_store; }
		std::vector<std::string>		getIndex(void) { return _index; };
		std::vector<std::string>		getServerName(void) { return _server_name; };
		long							getClientMaxBodySize(void)	{ return _client_max_body_size; };
		std::map<int, std::string>&		getErrorPage(void) { return _error_page; };
		std::vector<LocationConfig>&	getLocations(void) { return _locations; };
		bool							getAutoindex(void) { return _autoindex; };
		bool							getAutoindexDefine(void) { return _autoindexfound; };
		bool							getCookies(void) {return this->_cookies;};

		void	displayServConf(void);
        //=== func ===
        LocationConfig*								matchLocation(const std::string& path);
		std::map<int, std::string >::const_iterator	findErrorPage(int key) const;
		//==== debug ====
        void	addLocation(LocationConfig loc) { _locations.push_back(loc); }
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
bool						parserequests(const char *buff, ssize_t bytes);
void						parse_autoindex_server	(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_root_server(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_autoindex_server	(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_root_server(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_cookie(Config *config, std::vector<std::string> *tokens, size_t *index, LocationConfig *locconfig, ServerConfig *servconf);
void						parse_cookie_server(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);

#endif
