#include "../../../includes/http/HttpResponse.hpp"

void	HttpResponse::_buildRedirResponse(std::string new_path)
{
	this->_status_code = 301;
	this->_status_message = "Moved Permanently";

	this->_headers.clear();
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Location", new_path));
	this->_headers.insert(std::make_pair("Content-Length", "0"));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
}

void	HttpResponse::_buildGetResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, std::map<int, Socket *> &map_socket, const int &epollfd, Connection &target)
{
	if (this->_isDone)
    	return;

	std::string	root;
	if (location && !location->getRoot().empty())
		root = location->getRoot();
	else
		root = servConf.getRoot();
	if (root.empty())
		return (_buildErrorResponse(500, servConf, location));
	std::string req_path = root + req.getPath();
	req_path = _clearPathGarbage(req_path);
	/* chemin ou dossier vide ? */
	struct stat s;
	const char *path = req_path.c_str();

	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
	{
		/* cas 301 */
		if (req_path.empty() || req.getPath()[req.getPath().size() - 1] != '/')
			return (_buildRedirResponse(req.getPath() + '/'));
		std::string 				html_index;
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
					return (_buildErrorResponse(403, servConf, location));
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
            return (_buildAutoIndexResponse(req_path, req, servConf, location));
		else
			return (_buildErrorResponse(403, servConf, location));
	}
	if (access(req_path.c_str(), F_OK) == -1)
		return (_buildErrorResponse(404, servConf, location));	
	if (access(req_path.c_str(), R_OK) == -1)
		return (_buildErrorResponse(403, servConf, location));
	if (_isCgiRequest(req.getPath(), location) && !this->_isDone)
	{
		if (access(req_path.c_str(), F_OK) != 0)
		{
			std::cout << "je suis dans cgi build 2" << std::endl;
			_buildErrorResponse(404, servConf, location);
			_response = _buildStringResponse();
			return ;
		}
		if (access(req_path.c_str(), R_OK | X_OK) != 0)
		{
			_buildErrorResponse(403, servConf, location);
			_response = _buildStringResponse();
			return ;
		}
		// std::cout << "j'ai passe les tests" << std::endl;
		_cgiBuild(req, servConf, location, epollfd, target, map_socket);
		return ;
	}
	else if (!this->_isDone)
	{
		std::ifstream			infile(req_path.c_str(), std::ios::binary | std::ios::in | std::ios::ate);
		if (!infile.is_open())
			return (_buildErrorResponse(500, servConf, location));
		std::ifstream::pos_type	size;
		if (infile)
		{
			size = infile.tellg();
			if (size > 0)
			{
				this->_body.resize(size);
				infile.seekg(0, std::ios::beg);
				infile.read(&this->_body[0], size);
			}
		}
		this->_isDone = true;
	}
	this->_status_code = 200;
	this->_status_message = "OK";

	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Content-Type", _findContentType(req_path)));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
}

/*
	Patch :
		- erreur double / dans post
		- ajouter delete
		- les cookies
*/