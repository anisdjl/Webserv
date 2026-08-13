#include "../../includes/http/HttpResponse.hpp"

void	HttpResponse::_buildPostResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	// check size
	// if cgi ?
	
	std::string upload_path;
	if (location && !location->getUploadStore().empty())
		upload_path = location->getUploadStore();
	else if (!servConf.getUploadStore().empty())
		upload_path = servConf.getUploadStore();
	else
		return (_buildErrorResponse(403, servConf, location));
	std::string root;
	if (location && !location->getRoot().empty())
		root = location->getRoot();
	else
		root = servConf.getRoot();
	if (root.empty())
		return (_buildErrorResponse(500, servConf, location));

	if (upload_path[0] != '/' && root[root.size() - 1] != '/')
		upload_path = "/" + upload_path;
	else if (upload_path[0] == '/' && root[root.size() - 1] == '/')
		upload_path.erase(upload_path.begin());
	if (upload_path[upload_path.size() - 1] != '/')
		upload_path += "/";
	std::string	create_uri = upload_path;
	upload_path = root + upload_path;
	upload_path = _clearPathGarbage(upload_path);
	std::string file_name;
	std::string already_exist;
	std::string extension = _extensionFinder(req);
	if (access(upload_path.c_str(), W_OK | F_OK)) // verif folder
		return (_buildErrorResponse(500, servConf, location));
	do
	{
		file_name.clear();
		for (int i = 0; i < 6; ++i)
			file_name = file_name + static_cast<char>('a' + std::rand()%26);
		already_exist = upload_path + file_name + extension;
	}
	while (access(already_exist.c_str(), F_OK) == 0);
	upload_path = upload_path + file_name + extension;
	create_uri = create_uri + file_name + extension;
    std::ofstream	outfile(upload_path.c_str() ,std::ios::binary | std::ios::out);
	if (!outfile.is_open())
		return (_buildErrorResponse(500, servConf, location));
	outfile.write(req.getBody().c_str(), req.getBody().size());
	outfile.close();
	this->_status_code = 201;
	this->_status_message = "Created";
	this->_body = "File uploaded successfully" ;
	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	if (create_uri[0] != '/')
		create_uri = '/' + create_uri;
	create_uri = _clearPathGarbage(create_uri);
	this->_headers.insert(std::make_pair("Location", create_uri));
	this->_headers.insert(std::make_pair("Content-Type", "text/plain"));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
}
/*
	std::string req_path = root + req.getPath();
	struct stat s;
	const char *path = req_path.c_str();
	if (stat(path, &s) != 0 )
		return (_buildErrorResponse(404, servConf, location));
	if (!S_ISDIR(s.st_mode))
		return (_buildErrorResponse(403, servConf, location));
*/
	// if (location && req.getBody().size() > static_cast<size_t>(location->getClientMaxBodySize())) // NE DOIS JAMAIS etre negatif
	// 	return (_buildErrorResponse(413, servConf, location));
	// else if (req.getBody().size() > static_cast<size_t>(servConf.getClientMaxBodySize()))
	// 	return (_buildErrorResponse(413, servConf, location));
