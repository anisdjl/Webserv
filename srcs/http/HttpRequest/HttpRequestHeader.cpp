#include "../../../includes/http/HttpRequest.hpp"

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