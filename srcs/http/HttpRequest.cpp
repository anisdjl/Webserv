#include "../../includes/http/HttpRequest.hpp"
#include <map>

HttpRequest::HttpRequest()
: _state(INCOMPLETE), _avancement(NOT_STARTED), _error(0)
{};

HttpRequest::~HttpRequest(){};


std::string		HttpRequest::getHeader(std::string key) const
{
	for(std::map<std::string, std::string >::const_iterator it = _header.begin();
		it != _header.end(); ++it)
	{
		if (it->first == key)
			return it->second; 
	}
	return "";
}

void 	HttpRequest::resetRequest()
{
	this->_method.clear();
	this->_path.clear();
	this->_query_string.clear();
	this->_version.clear();
	this->_header.clear();
	this->_body.clear();
	this->_error = 0;
	this->_state = INCOMPLETE;
	this->_avancement = NOT_STARTED;
	this->_buffer.clear();
}


bool 	HttpRequest::_ft_parse_first_line()
{
	if (this->_method.empty() && this->_ft_skip_line(1))
		return (true);
	size_t pos = this->_buffer.find("\r\n");
	if(pos != std::string::npos)
	{
		std::string line = this->_buffer.substr(0, pos);
		if(ft_parse_method_and_stuff(line))
		 	return (true);
		else
		{
			this->_avancement = FIRST_LINE;
			return (false);
		}
	}
	return (true);
}

bool HttpRequest::_ft_parse_first_headers()
{
	while((pos = this->_buffer.find("\r\n") != std::string::npos))
	{
		
	}
}



void 	HttpRequest::ft_parse_http_request(const std::string& buffer)
{
	this->addToBuffer(buffer);
	if (this->_avancement == NOT_STARTED &&  this->_ft_parse_first_line())
		return ;
	else if (this->_avancement == FIRST_LINE && this->_ft_parse_headers()) 
		return ;
	else if (this->_avancement == HEADER && this->_ft_parse_body())
		return ;
	return ;
}