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

bool	HttpResponse::_buildGetResponse(HttpRequest& req,const ServerConfig &servConf,const LocationConfig *location, const int  &epollfd, std::map<int, Socket *> &map_socket, Connection &target)
{
	if (this->_isDone)
    	return false;
	std::string	root;
	std::string req_path;
	std::string index_path;

	if (!_BuildPath(req, req_path,root, index_path, servConf, location))
		return (false);
	if (_isCgiRequest(index_path, location) && !this->_isDone)
	{
		// std::cout << "j'ai passe les tests" << std::endl;
		_cgiBuild(req, servConf, location, epollfd, target, map_socket, req_path, index_path);
		return (true);
	}
	else if (!this->_isDone)
	{
		std::ifstream			infile(req_path.c_str(), std::ios::binary | std::ios::in | std::ios::ate);
		if (!infile.is_open())
			return (_buildErrorResponse(500, servConf, location), false);
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
	return false;
}
