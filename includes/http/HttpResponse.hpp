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
		ResponseState						getState();
		void								setState(ResponseState state);
		void                                resetResponse();
		void								buildResponse(Connection &target, ServerConfig &servConf,std::map<int, Socket *> &map_socket, int const &epollfd);
        /*	debug	*/
        std::string							getResponse() const;
        std::string							getBody() const;
		int         						getStatusCode() const;
        /*			*/
        void                                add_bytes_sent(unsigned int bytes);
        unsigned int                        get_bytes_sent() const;
        void                                setResponse(const std::string& response);
        void                                setBody(const std::string& body);
		/* do not touch */
		void								addString(std::string string) { _cgiresponse += string; };
		std::string							getResult(void) { return (_cgiresponse); };

	private:
		ResponseState						_state;
        int									_status_code;
        std::string							_status_message;
        unsigned int                        _bytes_sent;
        std::map<std::string, std::string>	_headers;
        std::string							_body;
		std::string							_response;
        std::string                         _findContentType(std::string path);
		std::string                         _extensionFinder(HttpRequest &req);
		std::string							_clearPathGarbage(std::string &path);
		std::string                         _buildStringResponse();
		/*  // do not touch   */
		std::string							_cgiresponse;
		/*                    */
        void                                _cgiBuild(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, int &epollfd);
        void								_buildAutoIndexResponse(std::string req_path, HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
		void								_buildErrorResponse(int error_code, ServerConfig &servConf, LocationConfig *location);
        bool								_isMethodAllowed(std::string path, LocationConfig *servConf);
        void								_buildRedirResponse(std::string new_path);
        void								_buildGetResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
        void								_buildPostResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
        void								_buildDeleteResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
};


char	**getEnv(HttpRequest &req, ServerConfig &servconf, LocationConfig *location);
char	*getPath(HttpRequest &req, ServerConfig &servconf, LocationConfig *location);
char	**getArgv(HttpRequest &req, ServerConfig &servconf, LocationConfig *location, char *path);

/*
    les fonctions necessaire devant traité:
        - get
        - post
        - delete
        - error : https://http.cat/
*/

#endif
