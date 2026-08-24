#include "../../../includes/http/HttpRequest.hpp"

ssize_t HttpRequest::_ft_verif_length(std::string &length, size_t &max_body_size)
{
	char *end;
	size_t content_length = std::strtoul(length.c_str(), &end, 10);
 
	if (*end != '\0' ) //length.empty() ||
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
	if ((pos = this->_buffer.find("0\r\n\r\n")) == std::string::npos || (pos > 0 && std::isdigit(this->_buffer[pos - 1])))
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