#ifndef HTTPRESPONSE
#define HTTPRESPONSE

#include "HttpRequest.hpp"
#include "../config/Config.hpp"

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
        void                                buildResponse(HttpRequest& request, ServerConfig &servConf);
		void                                resetResponse();
		ResponseState						getState();
		void								setState(ResponseState state);
        void                                add_bytes_sent(unsigned int bytes);
        unsigned int                        get_bytes_sent() const;
        std::string                         getResponse() const;
        void                                setResponse(const std::string& response);
        void                                setBody(const std::string& body);
        std::string                         getBody() const;
	private:
		ResponseState						_state;
        unsigned int                        _bytes_sent;
        int									_status_code;
        std::string							_status_message;
        std::map<std::string, std::string>	_headers;
        std::string							_body;
        std::string                         _findContentType(std::string path);
		std::string							_response;
		std::string                         _buildStringResponse();
        // void                                _cgiBuild(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, Socket socket);
        void								_buildErrorResponse(int error_code, ServerConfig &servConf, LocationConfig *location);
        bool								_isMethodAllowed(std::string path, LocationConfig *servConf);
        void								_buildGetResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
        void								_buildPostResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
        void								_buildDeleteResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location);
};

/*
    const char *response =
     "HTTP/1.1 200 OK\r\n"
     "Content-Type: text/plain\r\n"
     "Content-Length: 12\r\n"
     "\r\n"
     "Hello World";
    https://medium.com/@ryanradder0/building-an-http-server-from-scratch-a-journey-beyond-frameworks-28dc49e94a2a
*/

/*
    les fonctions necessaire devant traité:
        - get
        - post
        - delete
        - error : https://http.cat/
*/

#endif
