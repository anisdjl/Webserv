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

	if (access(req_path.c_str(), W_OK | F_OK))
		return (_buildErrorResponse(404, servConf, location));

	struct stat s;
	const char *path = req_path.c_str();
	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
		return (_buildErrorResponse(403, servConf, location));
	if (access(req_path.c_str(), F_OK) == -1)
		return (_buildErrorResponse(404, servConf, location));
	if (access(req_path.c_str(), R_OK) == -1)
		return (_buildErrorResponse(403, servConf, location));
	
	// delete

	// header
}