#ifndef HTTPRESPONSE
#define HTTPRESPONSE

#include "HttpRequest.hpp"
#include "../config/Config.hpp"
#include <sys/stat.h>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <dirent.h>
#include <ctime>

class Connection;
class Socket;

enum ResponseState
{
	NOT_BUILT,
	PROCESSING,
	BUILT
};

class HttpResponse
{
    public:
		HttpResponse();
        ~HttpResponse();
		const ResponseState					&getState() const {return (this->_state);}
		void								setState(ResponseState state);
		void								setHeader(std::string key, std::string value) {
			this->_headers.insert(std::make_pair(key, value));
		}
        bool								getisDone();
		void                                resetResponse();
		void								buildResponse(Connection &target,const ServerConfig &servConf,std::map<int, Socket *> &map_socket, int const &epollfd);
        /*	debug	*/
		const std::string					&getResponse() const { return (this->_response); }
		const std::string					&getBody() const { return (this->_body); }
		int         						getStatusCode() const;
        /*			*/
        void                                add_bytes_sent(unsigned int bytes);
        unsigned int                        get_bytes_sent() const;
        void                                setResponse(const std::string& response);
        void                                setBody(const std::string& body);
		void								addBody(std::string string) { _body.append(string); };

        void                                setisDone(bool state);
		std::string                         _findContentType(std::string path);
	private:
		ResponseState						_state;
        int									_status_code;
        std::string							_status_message;
        unsigned int                        _bytes_sent;
        std::map<std::string, std::string>	_headers;
        std::string							_body;
		std::string							_response;
        bool                                _isDone;
        std::map<std::string, std::string>	_header_cookie;
		std::string                         _extensionFinder(HttpRequest &req);

        void								_buildAutoIndexResponse(std::string req_path, HttpRequest& req, const ServerConfig &servConf, const LocationConfig *location);
		void								_buildErrorResponse(int error_code, const ServerConfig &servConf, const LocationConfig *location);
        bool								_isMethodAllowed(std::string path, const LocationConfig *servConf);
		bool								_isCgiRequest(std::string path, const LocationConfig *location) const;
        void								_buildRedirResponse(std::string new_path);

        bool	                            _buildGetResponse(HttpRequest& req,const ServerConfig &servConf,const LocationConfig *location, const int  &epollfd, std::map<int, Socket *> map_socket, Connection &target);
        bool								_buildPostResponse(HttpRequest& req, const ServerConfig &servConf, const LocationConfig *location, const int  &epollfd, std::map<int, Socket *> map_socket, Connection &target);
        void								_buildDeleteResponse(HttpRequest& req, const ServerConfig &servConf, const LocationConfig *location);
		bool								_cgiStartChecker(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);

		std::string							_clearPathGarbage(std::string &path);
		std::string                         _buildStringResponse();

        void								_cgiBuild(HttpRequest& req, const ServerConfig &servConf, const LocationConfig *location, const int &epollfd, Connection &target, std::map<int, Socket *> &map_socket);
        void                                _buildCookie(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
};


char	**getEnv(HttpRequest &req, const ServerConfig &servconf,const  LocationConfig *location);
char	*getPath(HttpRequest &req, const ServerConfig &servconf,const  LocationConfig *location);
char	**getArgv(HttpRequest &req, const ServerConfig &servconf,const  LocationConfig *location, char *path);

/*
    les fonctions necessaire devant traité:
        - get
        - post
        - delete
        - error : https://http.cat/
*/

#endif
