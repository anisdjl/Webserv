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
}

bool HttpRequest::_ft_skip_line()
{
	std::string temp;

	if(this->_buffer.length() >= 4)
	{
		temp = this->_buffer.substr(0, 3);
		if (temp == "\r\n\r\n")
			this->_buffer.erase(0, 3);
		return (false);
	}
	return (true)
}

bool 	HttpRequest::_ft_parse_request_line(size_t &pos)
{
	std::string line = this->_buffer.substr(0, pos + 2);
	std::istringstream iss(line);
	std::string temp;

	if (!(iss >> this->_method >> this->_path >> this->_version) || (iss >> temp))
	{
		this->setError(400);
		this->_state = COMPLETE;
		return (true);	
	}
	this->_buffer.erase(0, pos + 2);
	return (false);
}

bool 	HttpRequest::_ft_parse_first_line()
{
	if (this->_method.empty() && this->_ft_skip_line())
		return (true);
	size_t pos = this->_buffer.find("\r\n");
	if(pos != std::string::npos)
	{
		if(this->_ft_parse_request_line(pos))
		 	return (true);
		else
		{
			this->_avancement = FIRST_LINE;
			return (false);
		}
	}
	return (true);
}

bool HttpRequest::_ft_parse_line_header(size_t &pos)
{
	std::string line = this->_buffer.substr(0, pos);
	std::istringstream iss(line);
	std::string key;
	std::string value;
	size_t pos_value = line.find(':');
	size_t start;

	if (pos_value == std::string::npos || line[pos_value + 1] == 0)
	{
		this->setError(400);
		this->_state = COMPLETE;
		return (true);	
	}
	key = line.substr(0, pos_value);
	start = key.find_first_not_of(" \t");
    if (start != std::string::npos)
        key.erase(0, start);
    else
        key.clear();
	value = line.substr(pos_value + 1 , line.length());
	start = value.find_first_not_of(" \t");
    if (start != std::string::npos)
        value.erase(0, start);
    else
        value.clear();
	if(key.empty() || value.empty())
	{
		this->setError(400);
		this->_state = COMPLETE;
		return (true);	
	}
	this->_buffer.erase(0, pos + 2);
	this->_header.insert(std::make_pair(key, value));
	return (false);
}

bool HttpRequest::_ft_parse_header()
{
	size_t pos;

	while((pos = this->_buffer.find("\r\n")) != std::string::npos)
	{
		if(pos + 2 < this->_buffer.length() && this->_buffer[pos + 2] == '\r' && pos + 3 < this->_buffer.length() && this->_buffer[pos+3] == '\n')
		{
			this->_avancement = HEADER;
			return (false);
		}
		if (this->_ft_parse_line_header(pos))
			return (true);
	}
	return (true);
}

void HttpRequest::_ft_verif_length(std::string &length, size_t &max_body_size)
{
	if (length.empty())
	{
		this->setError(400);
		return ;
	}
	char *end;
	size_t content_length = std::strtoul(length.c_str(), &end, 10);
	if (*end != '\0' || content_length > max_body_size)
	{
		this->setError(400);
		return ;
	}
	if (this->_body.length() != content_length)
	{
		this->setError(413);
		return ;
	}
}

bool HttpRequest::_ft_parse_chunk(size_t &pos, std::string &new_body)
{}

void HttpRequest::_ft_unchunked(std::string &flags)
{
	size_t pos;
	std::string new_body;

	
	if(flags.find("chunked") == std::string::npos)
		return ;
	while((pos = this->_body.find("\r\n")) != std::string::npos)
	{
		if (this->_ft_parse_chunk(pos, new_body))
		{
			this->setError(400);
			return ;			
		}
	}
	this->_body = new_body;
}

void HttpRequest::_ft_check_flags_header(size_t &max_body_size)
{
	std::map<std::string , std::string>::iterator chunked_it = this->_header.find("Transfer-Encoding");
	std::map<std::string , std::string>::iterator length_it = this->_header.find("Content-Length");

	if((chunked_it != this->_header.end() && length_it != this->_header.end()) 
	|| (chunked_it == this->_header.end() && length_it == this->_header.end()))
	{
		this->setError(400);
		return ;
	}
	if (chunked_it != this->_header.end())
		this->_ft_unchunked(chunked_it->second);	
	else
		this->_ft_verif_length(length_it->second, max_body_size);
}

bool HttpRequest::_ft_parse_body(size_t &max_body_size)
{
	size_t pos;

	if(( pos = this->_buffer.find("\r\n\r\n")) == std::string::npos)
		return (true);
	this->_body = this->_buffer.substr(0, pos + 4);
	this->_buffer.erase(0, pos + 4);
	this->_ft_check_flags_header(max_body_size);
	this->_state = COMPLETE;
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