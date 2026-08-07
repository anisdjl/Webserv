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
        std::string                     _path;
        std::string                     _root;
        std::vector<std::string>        _index;
        std::vector<std::string>        _methods;
        bool                            _autoindex;
        std::vector<std::string>        _cgis;
        long                            _client_max_body_size;
        std::string                     _upload_store;
		std::string                     _return;
    public:
        LocationConfig();
        ~LocationConfig();
        std::string                     getPath() const;
        const std::vector<std::string>& getIndex() const;
        const std::vector<std::string>& getMethods() const;
        bool                            getAutoIndex() const;
        const std::vector<std::string>& getCgis() const;
        std::string                     getUploadStore() const;
        std::string                     getRoot() const;
        long                            getClientMaxBodySize() const;
        std::string                     getReturn() const;

        // === debug ===
        void setPath(std::string path) { _path = path; }
        void setRoot(std::string root) { _root = root; }
        void setAutoIndex(bool autoindex) { _autoindex = autoindex; }
        void addMethod(std::string method) { _methods.push_back(method); }

		void	clearMethods(void) { _methods.clear(); };
		void	displayLocation(void);
};

class	ServerConfig
{
    private:
        std::string                     _listen;
        std::string                     _host;
		std::string                     _root;
        std::vector<std::string>        _index;
        std::vector<std::string>        _server_name;
        std::string                     _upload_store;
        long                            _client_max_body_size;
        std::map<int, std::string>      _error_page;
        std::vector<LocationConfig>     _locations;

    public:
        ServerConfig();
        ~ServerConfig();
        // get
        std::string									getListen() const;
        std::string									getHost() const;
        const   std::vector<std::string>&			getServerName() const;
        const   std::vector<std::string>&           getIndex() const;
		std::string                     			getRoot() const;
        long                            			getClientMaxBodySize() const;
        std::map<int, std::string >::const_iterator	findErrorPage(int key) const;
		const std::map<int, std::string >			&getErrorPage() const;
        std::string									getUploadStore() const;
        const std::vector<LocationConfig>&			getLocations() const;
        // func
        LocationConfig*								matchLocation(const std::string& path);

        //==== debug ====
        void setRoot(std::string root) { _root = root; }
        void addLocation(LocationConfig loc) { _locations.push_back(loc); }
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
