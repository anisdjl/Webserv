#ifndef PARSING_HPP
# define PARSING_HPP

# include "webserv.hpp"

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


class Config
{
	private:


	public:
		Config(void);
		Config(const Config &src);
		Config	&operator=(const Config &src);
		~Config(void);
};

#endif