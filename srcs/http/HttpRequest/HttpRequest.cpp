#include "../../../includes/http/HttpRequest.hpp"


HttpRequest::HttpRequest() : _state(INCOMPLETE), _avancement(NOT_STARTED), _error(0){};

HttpRequest::~HttpRequest(){};

const std::string		&HttpRequest::getHeader(std::string key) const
{
	std::string n_key = this->_ft_tolower(key);
	
	std::map<std::string, std::string >::const_iterator it = _header.find(n_key);
	if (it != _header.end())
		return it->second;
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
	this->_cookies.clear();
	this->_error = 0;
	this->_state = INCOMPLETE;
	this->_avancement = NOT_STARTED;
}

void 	HttpRequest::ft_parse_http_request(const std::string& buffer, size_t max_body_size)
{
	this->addToBuffer(buffer);
	if (this->_avancement == NOT_STARTED &&  this->_ft_parse_first_line())
		return ;
	else if (this->_avancement == FIRST_LINE && this->_ft_parse_header()) 
		return ;
	else if (this->_avancement == HEADER && this->_ft_parse_body(max_body_size))
		return ;
	return ;
}