#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../webserv.hpp"

enum STATE
{
    SERVER_SECTION,
    LOCATION_SECTION,
    MAIN_SECTION
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
        std::string                     getReturn() const;
};

class ServerConfig
{
    private:
        std::string                     _listen;
        std::string                     _host;
		std::string                     _root;
        std::vector<std::string>        _server_name;
        long                            _client_max_body_size;
        std::map<int, std::string>      _error_page;
        std::vector<LocationConfig>     _locations;

    public:
        ServerConfig();
        ~ServerConfig();
        // get
        std::string									getListen() const;
        std::string									getHost() const;
        const std::vector<std::string>&				getServerName() const;
		std::string                     			getRoot() const;
        long                            			getClientMaxBodySize() const;
        std::map<int, std::string >::const_iterator	findErrorPage(int key) const;
		std::map<int, std::string >					getErrorPage() const;
        const std::vector<LocationConfig>&			getLocations() const;
        // func
        LocationConfig*								matchLocation(const std::string& path);
};

class Config
{
    private:
        STATE                           _state;
        std::vector<ServerConfig>       _servers;

    public:
        Config();
        ~Config();
        void                            setState(STATE state);
        STATE                           getState() const;
        const std::vector<ServerConfig>& getServers() const;
};

#endif
