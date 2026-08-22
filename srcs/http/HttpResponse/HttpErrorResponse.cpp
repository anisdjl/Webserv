#include "../../../includes/http/HttpResponse.hpp"

void	HttpResponse::_buildErrorResponse(int error_code,const ServerConfig &servConf,const LocationConfig *location)
{
	this->_status_code = error_code;
	switch (error_code)
	{
		case 400: // erreur dans le parsing
			this->_status_message = "Bad Request";
			break;
		case 403:
			this->_status_message = "Forbidden";
			break;
		case 404:
			this->_status_message = "Not Found";	
			break;
		case 405: // location est utile ici
			this->_status_message = "Method Not Allowed";
			break;
		case 413:
			this->_status_message = "Payload Too Large";
			break;
		case 501:
			this->_status_message = "Not Implemented";
			break;
		case 505:
			this->_status_message = "Version Not Supported";
			break;
		default:
			this->_status_message = "Internal Server Error";
			break;
	}
	std::string	root;
	std::string	html_path("");
	const std::map<int, std::string >&	error_page = servConf.getErrorPage();
	std::map<int, std::string >::const_iterator	it = servConf.findErrorPage(error_code);

	if (it != error_page.end())
	{
		if (location && !location->getRoot().empty())
			root = location->getRoot();
		else
			root = servConf.getRoot();
		html_path = root + it->second;
	} 

	std::ifstream	infile(html_path.c_str());
	// this->_body.clear();
	if (!infile.is_open())
	{
		this->_body = "<html>\n"
							"<head><title>" + this->_status_message + "</title></head>\n"
							"<body>\n"
								"<h1>" + this->_status_message + "</h1>\n"
							"</body>\n"
						"</html>";
	}
	else
	{
		std::string		line;
		while (std::getline(infile, line))
		{
			line += "\n";
			this->_body += line;
		}
		infile.close();
	}
	/* header */
	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.clear(); 
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Content-Type", "text/html"));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "close"));
}
