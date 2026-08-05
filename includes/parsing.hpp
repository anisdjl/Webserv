#ifndef PARSING_HPP
# define PARSING_HPP

# include "webserv.hpp"

class LocationConfig;
class ServerConfig;

class Request
{
	private:
		std::string							_start_line;
		std::map<std::string, std::string>	_header;
		std::string							_body;
		long								_content_length;

	public:
		Request(void);
		Request(const Request &src);
		Request &operator=(const Request &src);
		~Request(void);
};


#endif