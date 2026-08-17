#include "../../includes/http/HttpRequest.hpp"
#include <map>
#include <algorithm>

HttpRequest::HttpRequest() : _state(INCOMPLETE), _avancement(NOT_STARTED), _error(0){};

HttpRequest::~HttpRequest(){};

std::string		HttpRequest::getHeader(std::string key) const
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
	return (true);
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
	size_t pos_2 = this->_path.find('?');
	if (pos_2 != std::string::npos)
	{
		this->_query_string = this->_path.substr(pos_2 + 1);
		this->_path = this->_path.substr(0, pos_2);
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

std::string HttpRequest::_ft_tolower(const std::string& src) const
{
	std::string dest = src;
	int i = 0;

	while (dest[i])
	{
		dest[i] = std::tolower(dest[i]);
		i++;
	}
	return (dest);
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
	key = this->_ft_tolower(line.substr(0, pos_value));
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
		if(pos == 0)
		{
			if (this->_header.find("host") == this->_header.end())
			{
				this->setError(400);
				this->_state = COMPLETE;
				return (true);
			}
			this->_buffer.erase(0,2);
			this->_avancement = HEADER;
			return (false);
		}
		if (this->_ft_parse_line_header(pos))
			return (true);
	}
	return (true);
}

ssize_t HttpRequest::_ft_verif_length(std::string &length, size_t &max_body_size)
{
	char *end;
	size_t content_length = std::strtoul(length.c_str(), &end, 10);

	if (length.empty() || *end != '\0' )
	{
		this->setError(400);
		this->_state = COMPLETE;
		return(-1) ;
	}
	else if (content_length > max_body_size)
	{
		this->_state = COMPLETE;
		this->setError(413);
		return(-1) ;
	}
	if (this->_buffer.length() < content_length)
		return (-1);
	return (content_length);
}

bool HttpRequest::_ft_parse_with_length(std::string &length, size_t &max_body_size)
{
	ssize_t len = this->_ft_verif_length(length, max_body_size);
	if (len == -1)
		return (true);
	this->_body += this->_buffer.substr(0,len);
	this->_buffer.erase(0, len);
	this->_state = COMPLETE;
	return (false);
}

bool HttpRequest::_ft_parse_chunk(size_t &pos, size_t &max_body_size)
{
	char *end;
	std::string chunk_size_str = this->_buffer.substr(0, pos);
	size_t chunk_size = std::strtoul(chunk_size_str.c_str(), &end, 16);
	std::string chunk_data;

	if (*end != '\0')
	    return (this->setError(400), true);
	if (chunk_size == 0)
	{
		this->_buffer.erase(0, std::min (5, static_cast<int>(this->_buffer.length())));
		this->_state = COMPLETE;
		return (true);
	}
	if (this->_buffer.length() < chunk_size)
		return (true);
	this->_body += this->_buffer.substr(pos + 2, chunk_size);
	this->_buffer.erase(0, pos + 2 + chunk_size + 2);
	if (this->_body.length() > max_body_size)
	{
		this->_state = COMPLETE;
		this->setError(413);
		return (true);
	}
	return (false);
}

bool HttpRequest::_ft_parse_with_chunked(std::string &flags, size_t &max_body_size)
{
	size_t pos;
	
	if(flags.find("chunked") == std::string::npos)
	{
		this->_state = COMPLETE;
		this->setError(400);
		return(true) ;
	}
	if ((pos = this->_buffer.find("\r\n0\r\n")) == std::string::npos)
		return (true);
	while((pos = this->_buffer.find("\r\n")) != std::string::npos)
	{
		if (this->_ft_parse_chunk(pos, max_body_size))
			return (true);
	}

	return (false);
}

bool HttpRequest::_ft_parse_body(size_t &max_body_size)
{
	std::map<std::string , std::string>::iterator chunked_it = this->_header.find("transfer-encoding");
	std::map<std::string , std::string>::iterator length_it = this->_header.find("content-length");

	if (chunked_it != this->_header.end() && length_it != this->_header.end())
	{
		this->_state = COMPLETE;
		this->setError(400);
		return (true);
	}
	if ((chunked_it != this->_header.end() && this->_ft_parse_with_chunked(chunked_it->second, max_body_size)) 
	|| (length_it != this->_header.end() && this->_ft_parse_with_length(length_it->second, max_body_size)))
			return (true);
	this->_state = COMPLETE;
	return (false);
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