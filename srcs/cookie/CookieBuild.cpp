#include "../../includes/http/HttpResponse.hpp"

void    HttpResponse::_buildCookie(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	std::string path;
	bool session_cookie;
	if (location && location->getCookies())
	{
		session_cookie = location->getCookies();
		path = "PATH=" + location->getPath();
	}
	else
	{
		session_cookie = servConf.getCookies();
		path = "PATH=/";
	}
	if (!session_cookie)
		return ;
	std::string session_id;
	for (int i = 0; i < 6; ++i)
		session_id = session_id + static_cast<char>('a' + std::rand()%26);
	session_id = "session_id=" + session_id + ";";
	this->_header_cookie.push_back(session_id);
	this->_header_cookie.push_back(path);
	this->_header_cookie.push_back("Max-Age=3600");
}
// Set-Cookie: session_id=abc1234; Path=/