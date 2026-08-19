#include "../../../includes/http/HttpResponse.hpp"
#include "../../../includes/socket/Connection.hpp"

void			HttpResponse::buildResponse(Connection &target, ServerConfig &servConf,std::map<int, Socket *> &map_socket, int const &epollfd)
{
	HttpRequest &request = target.getHttpRequest();

	(void)map_socket;
	(void)epollfd;
   	if (request.getErrorCode() != 0)
    {
        this->_buildErrorResponse(request.getErrorCode(), servConf, NULL);
		_response = _buildStringResponse();
		return ;
    }
	if (request.getVersion() != "HTTP/1.1")
	{
		this->_buildErrorResponse(505, servConf, NULL);
		_response = _buildStringResponse();
		return ;
	}
	if (request.getMethod() != "GET" && request.getMethod() != "POST" && request.getMethod() != "DELETE")
	{
		this->_buildErrorResponse(501, servConf, NULL);
		_response = _buildStringResponse();
		return ;
	}
    LocationConfig *location = servConf.matchLocation(request.getPath());
	if (location && !this->_isMethodAllowed(request.getMethod(), location)) // check droit
	{
		this->_buildErrorResponse(405, servConf, location);
		_response = _buildStringResponse();

		return ;
	}
    if (request.getMethod() == "GET")
        this->_buildGetResponse(request, servConf, location);
    else if (request.getMethod() == "POST")
        this->_buildPostResponse(request, servConf, location);
    else if (request.getMethod() == "DELETE")
        this->_buildDeleteResponse(request, servConf, location);
    else
		this->_buildErrorResponse(501, servConf, location); // not found
	_response = _buildStringResponse();
}

// for (size_t i = 0; i < location->getMethods().size(); i++)

std::string		HttpResponse::_buildStringResponse()
{
    std::stringstream ss;

    ss << "HTTP/1.1 " << this->_status_code 
		<< " " << this->_status_message << "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
         it != this->_headers.end(); ++it)
    {
        ss << it->first << ": " << it->second << "\r\n";
    }
    ss << "\r\n";
    ss << this->_body;
	// /r pour la norme http
	this->_state = BUILT;
    return (ss.str());
}

void			HttpResponse::_buildAutoIndexResponse(std::string path, HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	std::vector<std::string> file;

	DIR *dir= opendir(path.c_str());
	if (!dir)
		return (_buildErrorResponse(500, servConf, location));
	dirent *pdir;
	while((pdir = readdir(dir)))
		file.push_back(pdir->d_name);
	closedir(dir);
	this->_body = "<html>\n"
						"<head><title>" + req.getPath() + "</title></head>\n"
						"<body>\n"
							"<hr>"
							"<pre>"
							"<h1> " + req.getPath() + " </h1>\n";
	for (std::vector<std::string>::const_iterator it = file.begin(); 
		it !=  file.end(); ++it)
		this->_body += "<a href=\"" + *it + "\">" + *it +"</a>\n";
	this->_body +=	"</pre>";
	this->_body += "</body>\n";
	this->_body +=	"</html>";
	
	this->_status_code = 200;
    this->_status_message = "OK";
	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Content-Type", "text/html"));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
}
