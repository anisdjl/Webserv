#include "../../includes/parsing.hpp"

// bool	ft_parse_request(Connection &target, const ServerConfig &config, const char *buffer, ssize_t bytesRead)
// {
// 	(void)target, (void) config;
// 	std::cout << "bytes size: " << bytesRead << " buffer: " << buffer << std::endl;



// 	return (true);
// }

bool						parserequests(const char *buff, ssize_t bytes)
{
	//std::cout << "bytes size: " << bytes << "\nbuffer: \n" << buff << std::endl;
	(void)bytes;
	
	std::string buffer = buff;
	size_t pos;
	size_t begin = 0;
	
	std::vector<std::string>	line_header;
	std::string					body;
	while ((pos = buffer.find("\r\n")) != std::string::npos)
	{
		std::string line = buffer.substr(begin, pos);
		if (line.empty())
		{
			buffer.erase(0, 2);
			break;
		}
		line_header.push_back(line);
		buffer.erase(0, pos + 2);
	}
	// tout ce qui reste ici est le body (si il reste un truc)

	for (std::vector<std::string>::iterator it = line_header.begin(); it != line_header.end(); ++it)
		std::cout << *it << std::endl;

	if (!buffer.empty()) body = buffer;
	else body = "";
	

	
	return true;
}
