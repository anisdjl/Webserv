#include "../../includes/http/HttpResponse.hpp"

void    HttpResponse::_buildCookie(HttpRequest& req, ServerConfig &servConf, LocationConfig *location)
{
	(void)req; (void)servConf; (void)location;
}
// Set-Cookie: session_id=abc1234; Path=/; Max-Age=3600