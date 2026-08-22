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
		void								setHeader(std::string key, std::string value) {
			this->_headers.insert(std::make_pair(key, value));
		}
        bool								getisDone();
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
		void								addBody(std::string string) { _body.append(string); };
		/* do not touch */
		void								addString(std::string string) { _cgiresponse += string; };
		std::string							getResult(void) { return (_cgiresponse); };

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
		bool								_cgiStartChecker(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);

		std::string                         _extensionFinder(HttpRequest &req);
		std::string							_clearPathGarbage(std::string &path);
		std::string                         _buildStringResponse();
		/*  // do not touch   */
		std::string							_cgiresponse;
		/*                    */
       	void								_cgiBuild(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, const int &epollfd, Connection &target, std::map<int, Socket *> &map_socket);
        void								_buildAutoIndexResponse(std::string req_path, HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
        void                                _buildCookie(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
		void								_buildErrorResponse(int error_code, ServerConfig &servConf, LocationConfig *location);
        bool								_isMethodAllowed(std::string path, LocationConfig *servConf);
		bool								_isCgiRequest(std::string path, LocationConfig *location) const;
        void								_buildRedirResponse(std::string new_path);
		void								_buildGetResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, std::map<int, Socket *> &map_socket, const int &epollfd, Connection &target);
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
