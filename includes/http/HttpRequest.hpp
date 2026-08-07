#ifndef HTTPREQUEST
#define HTTPREQUEST

#include "../Webserv.hpp"

enum RequestState
{
	INCOMPLETE,
	COMPLETE,
	ERROR
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

/*
HTTP/1.1 201 Created
Content-Type: application/json
Location: http://example.com/users/123

{
  "message": "New user created",
  "user": {
    "id": 123,
    "firstName": "Example",
    "lastName": "Person",
    "email": "bsmth@example.com"
  }
}
*/
// chaque chemin a ses regles def dans location
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Methods
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Messages

#endif
