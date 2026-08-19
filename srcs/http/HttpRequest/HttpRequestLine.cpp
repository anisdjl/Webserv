#include "../../../includes/http/HttpRequest.hpp"

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
