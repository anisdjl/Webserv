#ifndef HTTPREQUEST
#define HTTPREQUEST

#include <iostream>
#include <map>

// Exemple possible de request http :
/*
	POST /cgi-bin/upload.py?user=42&action=save HTTP/1.1\r\n
	Host: localhost:8080\r\n
	User-Agent: Mozilla/5.0 (X11; Linux x86_64)\r\n
	Content-Type: application/x-www-form-urlencoded\r\n
	Content-Length: 27\r\n
	Cookie: session_id=abc123xyz\r\n
	\r\n
	name=JohnDoe&age=25&status=ok
*/

class HttpRequest
{
    public:
        HttpRequest();
    	~HttpRequest();
		std::string	                        	getMethod() const;
		std::string                         	getPath() const;
		std::string                         	getQueryString() const;
		std::string                         	getVersion() const;
		std::map<std::string, std::string>  	getHeaders() const;
		std::string                         	getBody() const;
		int										getErrorCode() const;
		std::string								getHeader(std::string& key) const;

		void									setMethod(std::string method);
		void 									setPath(std::string path);
		void 									setQueryString(std::string query);
		void 									setVersion(std::string version);
		void									setHeader(std::string key, std::string value);
		void 									setBody(std::string body);
		void									setError(int code);
	private:
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