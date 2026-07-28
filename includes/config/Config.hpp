#ifndef CONFIG
# define CONFIG

#include <string>

typedef struct s_serveur_config
{
	std::string _listen;
	std::string _host;
	std::string _server_name;
	std::string _root;
	std::string _index;
	std::string _client_body_size; // max
	std::string _server_name;
	std::string _error_page;
}	t_server;

typedef struct s_location
{
	std::string _allow_methods;
	std::string _auto_index;
	std::string _upload_to;
	std::string _cgi_path;
	std::string _cgi_extension;
	std::string _return;
}	t_location;

class Config
{
	public:
		Config(void);
		~Config();
		// serveur
		std::string 	getListen() const;
		std::string 	getHost() const;
		std::string		getServerName() const;
		std::string		getRoot() const;
		std::string		getIndex() const;
		std::string		getClientBodySize() const;
		std::string		getErrorPage() const;
		// location
		std::string		getAllowMethods() const;
		std::string		getAutoIndex() const;
		std::string		getUploadTo() const;
		std::string		getCgiPath() const;
		std::string		getCgiExtention() const;
		std::string		getReturn() const;
	private :
		t_server 		server;
		t_location 		location;

};

#endif