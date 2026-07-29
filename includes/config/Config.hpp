#ifndef CONFIG
# define CONFIG

#include <string>
#include <vector>

typedef struct s_serveur_config
{
	int			 				_listen; // port
	std::string					_host; 
	std::string 				_server_name;
	std::string 				_root;
	std::string		 			_index;
	int		 					_client_body_size; // max
	std::string 				_error_page;
}								t_server;

typedef struct s_location
{
	std::string					_location_url; // /upload par exemple
	std::string 				_allow_methods;
	bool 						_auto_index;
	std::string 				_upload_to;
	std::string 				_cgi_path;
	std::string 				_cgi_extension;
	std::string 				_return;
}								t_location;
// location definit les regles par zone

class Config
{
	public:
		Config(void);
		~Config();
		// serveur
		int			 	getListen() const;
		std::string 	getHost() const;
		std::string		getServerName() const;
		std::string		getRoot() const;
		std::string		getIndex() const;
		int				getClientBodySize() const;
		std::string		getErrorPage() const;
		// location
		std::string		getLocationUrl(int index) const;
		std::string		getAllowMethods(int index) const;
		bool			getAutoIndex(int index) const;
		std::string		getUploadTo(int index) const;
		std::string		getCgiPath(int index) const;
		std::string		getCgiExtention(int index) const;
		std::string		getReturn(int index) const;
		t_location    	*matchLocation(std::string path);
	private :
		t_server 					server;
		std::vector<t_location> 	location;
};	

#endif