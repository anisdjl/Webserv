#include "../../includes/http/HttpResponse.hpp"

void	HttpResponse::_buildDeleteResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	std::string root;
	if (location && !location->getRoot().empty())
		root = location->getRoot();
	else
		root = servConf.getRoot();
	if (root.empty())
		return (_buildErrorResponse(500, servConf, location));
	std::string req_path = root + req.getPath();
	req_path = _clearPathGarbage(req_path);
	if (access(req_path.c_str(), F_OK) == -1)
		return (_buildErrorResponse(404, servConf, location));
	struct stat s;
	if (stat(req_path.c_str(), &s) == 0 && S_ISDIR(s.st_mode))
		return (_buildErrorResponse(403, servConf, location));
	if (access(req_path.c_str(), W_OK) == -1)
		return (_buildErrorResponse(403, servConf, location));
	
	if (std::remove(req_path.c_str()) != 0) // ou unlink
		return (_buildErrorResponse(500, servConf, location));
	this->_status_code = 200;
	this->_status_message = "OK";
	this->_body = "File deleted successfully" ;
	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Content-Type", "text/plain"));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
}