
#include "../../../includes/http/HttpResponse.hpp"

bool ft_root(std::string &root, const ServerConfig &servConf, const LocationConfig *location)
{
	if (location && !location->getRoot().empty())
		root = location->getRoot();
	else if (!servConf.getRoot().empty())
		root = servConf.getRoot();
	else
		return (false);
	return (true);
}


bool HttpResponse::_BuildPath(HttpRequest &req, std::string &req_path, std::string	&root, std::string &index_path,const ServerConfig &servConf, const LocationConfig *location)
{
	struct stat s;
	std::string html_index;

	if (!ft_root(root, servConf, location))
		return (_buildErrorResponse(500, servConf, location), false);
	req_path = root + req.getPath();
	req_path = _clearPathGarbage(req_path);
	const char *path = req_path.c_str();

	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
	{
		/* cas 301 */
		if (req_path.empty() || req.getPath()[req.getPath().size() - 1] != '/')
			return (_buildRedirResponse(req.getPath() + '/'), false);

		std::vector<std::string>	index_vector;

		if (location && !location->getIndex().empty())
			index_vector = location->getIndex();
		else if (!servConf.getIndex().empty())
			index_vector = servConf.getIndex();
		else
			index_vector.push_back("index.html");
		if (req_path[req_path.size() - 1] != '/')
        	req_path += "/";
		for (std::vector<std::string>::const_iterator it = index_vector.begin();
			it !=  index_vector.end(); ++it)
		{
			if (access((req_path + *it).c_str(), F_OK) == 0)
			{
				if (access((req_path + *it).c_str(), R_OK) != 0)
					return (_buildErrorResponse(403, servConf, location), false);
				html_index = *it;
				break;
			}
		}

		bool auto_index;
		if (location && location->getAutoindexDefine())
			auto_index = location->getAutoindex();
		else if (servConf.getAutoindexDefine())
			auto_index = servConf.getAutoindex();
		else
			auto_index = false;
		if (!html_index.empty())
		{
			req_path = _clearPathGarbage(req_path);
			req_path += html_index;
		}
		else if (auto_index)
            return (_buildAutoIndexResponse(req_path, req, servConf, location), false);
		else
			return (_buildErrorResponse(403, servConf, location), false);
	}
	if (access(req_path.c_str(), F_OK) == -1)
		return (_buildErrorResponse(404, servConf, location), false);
	if (access(req_path.c_str(), R_OK) == -1)
		return (_buildErrorResponse(403, servConf, location), false);
	index_path = req.getPath() + html_index;
	return (false);
}
