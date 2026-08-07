#include "../../includes/http/HttpResponse.hpp"

HttpResponse::HttpResponse() : _status_code(200), _status_message("OK"), _headers(), _body("") , _bytes_sent(0) {} // tmp

HttpResponse::~HttpResponse(){};

std::string		HttpResponse::getResponse() const
{
	return (this->_response);
}

std::string		HttpResponse::getBody() const
{
	return (this->_body);
}

int         	HttpResponse::getStatusCode() const
{
	return (this->_status_code);
}

void HttpResponse::buildResponse(HttpRequest& request, ServerConfig &servConf)
{
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
    else // method not allowed /
        this->_buildErrorResponse(501, servConf, location); // not found
	_response = _buildStringResponse();
}

// for (size_t i = 0; i < location->getMethods().size(); i++)

std::string HttpResponse::_buildStringResponse()
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
    return (ss.str());
}

void	HttpResponse::_buildAutoIndexResponse(std::string path, HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
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
} // ignoré . ?

// https://en.angie.software/angie/docs/configuration/modules/http/http_autoindex/
// https://www.codespeedy.com/get-all-the-files-in-a-directory-using-cpp/

void	HttpResponse::_buildGetResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	std::string	root;
	if (location && !location->getRoot().empty())
		root = location->getRoot();
	else
		root = servConf.getRoot();
	std::string req_path = root + req.getPath();

	/* chemin ou dossier vide ? */
	struct stat s;
	const char *path = req_path.c_str();
	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
	{
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
		if (!html_index.empty())
			req_path += html_index;
		else if (location && location->getAutoindex() == true)
			return (_buildAutoIndexResponse(req_path, req, servConf, location));
		else
			return (_buildErrorResponse(403, servConf, location));
	}
	if (access(req_path.c_str(), F_OK) == -1)
		return (_buildErrorResponse(404, servConf, location));
	if (access(req_path.c_str(), R_OK) == -1)
		return (_buildErrorResponse(403, servConf, location));
	this->_status_code = 200;
	this->_status_message = "OK";

	// if cgi => fonction vers cgiBuild

	std::ifstream			infile(req_path.c_str(), std::ios::binary | std::ios::in | std::ios::ate);
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
	
	std::ostringstream oss;
	oss << this->_body.size();
	this->_headers.insert(std::make_pair("Server", "WeebServ"));
	this->_headers.insert(std::make_pair("Content-Type", _findContentType(req_path)));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
}

void	HttpResponse::_buildPostResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	if (location && req.getBody().size() > location->getClientMaxBodySize())
		return (_buildErrorResponse(413, servConf, location));
	else if (req.getBody().size() > servConf.getClientMaxBodySize())
		return (_buildErrorResponse(413, servConf, location));

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
	upload_path = root + upload_path;
	std::string file_name;
	for (int i = 0; i < 6; ++i)
		file_name = file_name + static_cast<char>('a' + std::rand()%26);
	if (access(upload_path.c_str(), W_OK | F_OK))
		return (_buildErrorResponse(500, servConf, location));
	upload_path = upload_path + file_name + _extensionFinder(req);
    std::ofstream outfile(upload_path.c_str() ,std::ios::binary | std::ios::out);
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
	this->_headers.insert(std::make_pair("Location", upload_path));
	this->_headers.insert(std::make_pair("Content-Type", "text/plain"));
	this->_headers.insert(std::make_pair("Content-Length", oss.str()));
	this->_headers.insert(std::make_pair("Connection", "keep-alive"));
}

	/* 
		c'est un cgi ? oui -> lancer buildCgi
					   non -> continuer

		trouver l'upload store serv ou loc
	 	si vide alors erreur

		
		crée le fichier + nom etc
		code 201 + header
	*/

void	HttpResponse::_buildDeleteResponse(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{

}

void	HttpResponse::_buildErrorResponse(int error_code, ServerConfig &servConf, LocationConfig *location)
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

void HttpResponse::resetResponse()
{
	this->_status_code = 200;
	this->_status_message = "OK";
	this->_headers.clear();
	this->_body.clear();
	this->_state = NOT_BUILT;
	this->_response.clear();
	this->_bytes_sent = 0;
}

ResponseState	HttpResponse::getState()
{
	return this->_state;
}

void	HttpResponse::setState(ResponseState state)
{
	this->_state = state;
}

void HttpResponse::add_bytes_sent(unsigned int bytes)
{
	this->_bytes_sent += bytes;
}

unsigned int HttpResponse::get_bytes_sent() const
{
	return this->_bytes_sent;
}

std::string HttpResponse::getResponse() const
{
	return this->_response;
}

void HttpResponse::setResponse(const std::string& response)
{
	this->_response = response;
}

std::string HttpResponse::getBody() const
{
	return this->_body;
}

void HttpResponse::setBody(const std::string& body)
{
	this->_body = body;
}

/*
	Location
	location->root // possible

	ServConf
	getErrorCodePage
	findErrorPage
	matchLocation

	il faut add la verife avant le build

	parser :
	400
	413

	Traiter les chemins absolues ?
	manque un / sur le debut ? get
	traiter les doubles /
	root vide partout
	manque cas avec cgi
*/

// void	HttpResponse::_cgiBuild(HttpRequest& req, ServerConfig &servConf, LocationConfig *location, Socket socket)
// {
// 	int fd;
// 	int pipe_in[2];
// 	int pipe_out[2];

// 	// creer pipe
// 	// socketpair (pipe_in[0], pipe_out[1])= merge socket;
// 	// fd = pipe_in[0] ou pipe_out[1]
// 	// in : recevoir
// 	// out :
// 	/*
// 		class socket :
// 				fd
// 				type : CGI
// 				parent_fd = fd du parent
// 		add au epoll
// 		add a la mapsocket
// 		fork()
// 		{
// 		tu fais ta magie
// 		}
// 	*/
// }
// int &epollfd, int &parent_fd, std::map<int, Socket> &map_socket
