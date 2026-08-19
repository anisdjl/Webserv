#include "../../../includes/http/HttpRequest.hpp"

bool HttpRequest::_ft_skip_line()
{
	std::string temp;

	if(this->_buffer.length() >= 4)
	{
		temp = this->_buffer.substr(0, 4);
		if (temp == "\r\n\r\n")
			this->_buffer.erase(0, 3);
		return (false);
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

