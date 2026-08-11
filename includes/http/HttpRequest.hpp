#ifndef HTTPREQUEST
#define HTTPREQUEST

#include "../Webserv.hpp"


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

enum RequestState
{
	INCOMPLETE,
	COMPLETE,
	ERROR
};

enum avancementState
{
	NOT_STARTED,
	FIRST_LINE,
	HEADER,
	BODY
};

class HttpRequest
{
    public:
        HttpRequest();
    	~HttpRequest();
		std::string	                        	getMethod() const {return (this->_method);}
		std::string                         	getPath() const {return (this->_path);}
		std::string                         	getQueryString() const {return (this->_query_string);}
		std::string                         	getVersion() const {return (this->_version);}
		std::string                         	getBody() const {return (this->_body);}
		int										getErrorCode() const {return (this->_error);}
		std::string								getHeader(std::string key) const {return (this->_header.count(key) ? this->_header.at(key) : "");};
		RequestState							getState() const {return (this->_state);}
		avancementState							getAvancement() const {return (this->_avancement);}
		std::string								getBuffer() const {return (this->_buffer);}

		void									setMethod(std::string method){this->_method = method;};
		void 									setPath(std::string path){this->_path = path;}
		void 									setQueryString(std::string query){this->_query_string = query;}
		void 									setVersion(std::string version){this->_version = version;};
		void									setHeader(std::string key, std::string value){this->_header[key] = value;}
		void 									setBody(std::string body){this->_body = body;}
		void									setError(int code){this->_error = code;}
		void									setState(RequestState state){this->_state = state;};
		void 									setAvancement(avancementState state){this->_avancement = state;}
		void 									setBuffer(std::string buffer){this->_buffer = buffer;}

		void 									ft_parse_http_request(const std::string& buffer);
		void 									resetRequest();
		void 									addToBuffer(const std::string& data) { this->_buffer += data; }
	private:
		RequestState							_state;
		avancementState							_avancement;
		std::string								_buffer;
		std::string								_method; // POST
		std::string								_path;	// a coté de post
		std::string								_query_string; // les parametres possible
		std::string								_version; // ex : HTTP/1.1
		std::map<std::string, std::string>		_header; // donc host, user_agent, content-type, content-length, cookie
		std::string 							_body; 
		int										_error;

		bool 									_ft_parse_first_line();
		bool									_ft_parse_header();
		bool									_ft_parse_body();
		bool									_ft_skip_line();
		bool									_ft_parse_request_line(size_t &pos);
		bool									_ft_parse_line_header(size_t &pos);
		void									_ft_check_flags_header();
};

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
