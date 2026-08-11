#ifndef PARSING_HPP
# define PARSING_HPP

# include "webserv.hpp"

class LocationConfig;
class ServerConfig;
class HttpRequest;

enum RequestState
{
	INCOMPLETE,
	COMPLETE,
	ERROR
};


enum Socket_type
{
    LISTENER,
    CONNECTION,
    CGI
};


class HttpRequest
{
    public:
        HttpRequest();
    	~HttpRequest();
		std::string	                        	getMethod() const;
		std::string                         	getPath() const;
		std::string                         	getQueryString() const;
		std::string                         	getVersion() const;
		std::string                         	getBody() const;
		int										getErrorCode() const;
		std::string								getHeader(std::string key) const;
		RequestState							getState() const;

		void									setMethod(std::string method);
		void 									setPath(std::string path);
		void 									setQueryString(std::string query);
		void 									setVersion(std::string version);
		void									setHeader(std::string key, std::string value);
		void 									setBody(std::string body);
		void									setError(int code);
		void									setState(RequestState state);

		void 									resetRequest();
	private:
		RequestState							_state;
		std::string								_method; // POST
		std::string								_path;	// a coté de post
		std::string								_query_string; // les parametres possible
		std::string								_version; // ex : HTTP/1.1
		std::map<std::string, std::string>		_header; // donc host, user_agent, content-type, content-length, cookie
		std::string 							_body; 
		int										_error;
};


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
		bool								_autoindexfound;

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
		void	setAutoIndexfound(bool found);

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
		std::string					_root; // je viens de le rajouter a la demande d'evan
		std::string					_listen;
		std::string					_host;
		bool						_autoindex; // je viens de le rajouter a la demande d'evan
		std::vector<std::string>	_server_name;
		long						_client_max_body_size;
		std::map<int, std::string>	_error_page;
		std::vector<LocationConfig>	_locations;
		bool						_autoindexfound;

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
		void	setAutoindex(std::string &autoindex);
		void	setRoot(std::string &root);
		void	setAutoIndexfound(bool found);

		std::string					getListen(void) { return _listen; };
		std::string					getHost(void) { return _host; };
		std::vector<std::string>	getServerName(void) { return _server_name; };
		long						getClientMaxBodySize(void)	{ return _client_max_body_size; };
		std::map<int, std::string>	getErrorPage(void) { return _error_page; };
		std::vector<LocationConfig>	getLocations(void) { return _locations; };

		void	displayServConf(void);
};

class Socket
{
	protected:
		int         	_fd;
    	int         	_server_index;
    	Socket_type 	_type;
		
		Socket() : _fd(-1), _server_index(-1), _type(LISTENER) {};
		Socket(int fd, int server_index, Socket_type type)
		: _fd(fd), _server_index(server_index), _type(type) 
		{};
		Socket(const Socket& src) 
		: _fd(src._fd), _server_index(src._server_index), _type(src._type) 
		{};
		
	public :

		
		~Socket();
		int &getFd()
		{
			return (this->_fd);
		};
		int getFd() const
		{
			return (this->_fd);
		};
		int getServerIndex() const
		{
			return (this->_server_index);
		};
		Socket_type getType() const
		{
			return (this->_type);
		};
		void setFd(int fd)
		{
			this->_fd = fd;
		};
		void setServerIndex(int index)
		{
			this->_server_index = index;
		};
		void setType(Socket_type type)
		{
			this->_type = type;
		};
		Socket& operator=(const Socket& src)
		{
			if (this != &src)
			{
				this->_fd = src._fd;
				this->_server_index = src._server_index;
				this->_type = src._type;
			}
			return (*this);
		};
};


// class Connection : public Socket
// {
//     private:
//         	HttpRequest		_http_request;
// 		    HttpResponse	_http_response;
    
//     public:
//         void setHttpRequest(HttpRequest& request)
// 		{
// 			this->_http_request = request;
// 		};
// 		void setHttpResponse(HttpResponse& response)
// 		{
// 			this->_http_response = response;
// 		};
// 		HttpRequest& getHttpRequest() const
// 		{
// 			return (this->_http_request);
// 		};
// 		HttpResponse& getHttpResponse() const
// 		{
// 			return (this->_http_response);
// 		};
//         Connection() : Socket(), _http_request(HttpRequest()), _http_response(HttpResponse())
// 		{
// 			this->_type = CONNECTION;
// 		};
//         Connection(int fd, int server_index, HttpRequest& request, HttpResponse& response):
// 		Socket(fd, server_index, CONNECTION), _http_request(request), _http_response(response)
// 		{};
//         Connection(const Connection& src)
// 		 : Socket(src), _http_request(src._http_request), _http_response(src._http_response)
// 		{};
//         ~Connection()
// 		{};
//         Connection& operator=(const Connection& src)
// 		{
// 			if (this != &src)
// 			{
// 				this->_fd = src._fd;
// 				this->_server_index = src._server_index;
// 				this->_type = src._type;
// 				this->_http_request = src._http_request;
// 				this->_http_response = src._http_response;
// 			}
// 			return (*this);
// 		};
// };

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
// bool						ft_parse_request(Connection &target, const ServerConfig &config, const char *buffer, ssize_t bytesRead);
bool						parserequests(const char *buff, ssize_t bytes);
void						parse_autoindex_server	(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);
void						parse_root_server(Config *config, std::vector<std::string> *tokens, size_t *index, ServerConfig *servconf);

#endif