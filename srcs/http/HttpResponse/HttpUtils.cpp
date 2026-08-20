#include "../../../includes/http/HttpResponse.hpp"

std::string	HttpResponse::_extensionFinder(HttpRequest &req)
{
	std:: string type = req.getHeader("Content-Type");
	
	if (type == "text/html")
		return (".html");
	else if (type == "text/css")
		return (".css");
	else if (type == "text/javascript")
		return (".js");
	else if (type == "application/json")
		return (".json");
	else if (type == "image/png")
		return (".png");
	else if (type == "image/jpeg")
		return (".jpg");
	else if (type == "image/svg+xml")
		return (".svg");
	else if (type == "image/webp")
		return (".webp");
	else if (type == "image/avif")
		return (".avif");
	else if (type == "image/gif")
			return (".gif");
	else if (type == "application/pdf")
		return (".pdf");
	else if (type == "text/plain")
		return (".txt");
	else if (type == "application/zip")
		return (".zip");
	return ("");
}

std::string	HttpResponse::_findContentType(std::string path)
{
	if (path.empty())
		return ("application/octet-stream");
	size_t pos = path.rfind(".");
	if (pos == std::string::npos || pos == 0)
		return ("application/octet-stream");
	std::string	extension = path.substr(pos + 1);
	
	if (extension == "html")
		return ("text/html");
	else if (extension == "css")
		return ("text/css");
	else if (extension == "js")
		return ("text/javascript");
	else if (extension == "json")
		return ("application/json");
	else if (extension == "png")
		return ("image/png");
	else if (extension == "jpeg" || extension == "jpg")
		return ("image/jpeg");
	else if (extension == "svg")
		return ("image/svg+xml");
	else if (extension == "webp")
		return ("image/webp");
	else if (extension == "avif")
		return ("image/avif");
	else if (extension == "gif")
			return ("image/gif");
	else if (extension == "pdf")
		return ("application/pdf");
	else if (extension == "txt")
		return ("text/plain");
	else if (extension == "zip")
		return ("application/zip");
	return ("application/octet-stream");
}

bool	HttpResponse::_isMethodAllowed(std::string methode, LocationConfig *location)
{
	std::vector<std::string> methods = location->getMethods();

	for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it)
	{
		if (*it == methode)
			return (true);
	}
	return (false);
}


std::string	HttpResponse::_clearPathGarbage(std::string &path)
{
	std::string	clean_path;
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] == '/' && i > 0 && path[i - 1] == '/')
			continue;
		else
			clean_path += path[i];
	}
	return (clean_path);
}

bool		HttpResponse::_cgiStartChecker(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	if (!location || location->getCgis().empty())
		return false;
	std::string file_name = req.getPath();
	// autre securité a ajouter ?
	std::string extension = file_name.substr(file_name.find_last_of("."));
	if (location->getCgis().find(extension) != location->getCgis().end())
		return true;
	return false;
}