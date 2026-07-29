#include "../../includes/config/Config.hpp"

Config::Config(){};

Config::~Config(){};

/* Server conf get */

int		Config::getListen() const
{
	return this->server._listen;
}

std::string	Config::getHost() const
{
	return this->server._host;
}

std::string	Config::getServerName() const
{
	return this->server._server_name;
}

std::string	Config::getRoot() const
{
	return this->server._root;
}

std::string	Config::getIndex() const
{
	return this->server._index;
}

int			Config::getClientBodySize() const
{
	return this->server._client_body_size;
}

std::string	Config::getErrorPage() const
{
	return this->server._error_page;
}

/* Location conf get */

std::string Config::getLocationUrl(int index) const
{
	return this->location[index]._location_url;
}

std::string	Config::getAllowMethods(int index) const
{
	return this->location[index]._allow_methods;
}

bool	Config::getAutoIndex(int index) const
{
	return this->location[index]._auto_index;
}

std::string	Config::getUploadTo(int index) const
{
	return this->location[index]._upload_to;
}

std::string	Config::getCgiPath(int index) const
{
	return this->location[index]._cgi_path;
}

std::string	Config::getCgiExtention(int index) const
{
	return this->location[index]._cgi_extension;
}

std::string	Config::getReturn(int index) const
{
	return this->location[index]._return;
}

// func

t_location*   Config::matchLocation(std::string path)
{
	t_location	*match;
	int			match_len;
	for (size_t i = 0; i < location.size(); i++)
	{
		std::string location_url = location[i]._location_url;
		if (path.compare(0, location_url.size(), location_url) == 0)
		{
			if (location_url.size() > match_len) // nv fav trouver
			{
				match = &location[i];
				match_len = location_url.size();
			}
		}
	}
	return (match);
}

/*
    if ((str1.compare(3, 5, str2)) == 0)
        cout << "Substring Matched";
    else
        cout << "Strings Not Matched";
*/