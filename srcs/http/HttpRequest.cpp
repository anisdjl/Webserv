#include "../../includes/http/HttpRequest.hpp"
#include <map>

HttpRequest::HttpRequest()
: _state(INCOMPLETE)
{};

HttpRequest::~HttpRequest(){};

std::string		HttpRequest::getMethod() const
{
    return this->_method;
}

std::string		HttpRequest::getPath() const
{
    return this->_path;
}

std::string		HttpRequest::getQueryString() const
{
    return this->_query_string;
}

std::string		HttpRequest::getVersion() const
{
    return this->_version;
}

std::string	HttpRequest::getBody() const
{
    return this->_body;
}

int		HttpRequest::getErrorCode() const
{
    return this->_error;
}

std::string		HttpRequest::getHeader(std::string key) const
{
	for(std::map<std::string, std::string >::const_iterator it = _header.begin();
		it != _header.end(); ++it)
	{
		if (it->first == key)
			return it->second; 
	}
	return "";
}

RequestState	HttpRequest::getState() const
{
	return this->_state;
}

void	HttpRequest::setMethod(std::string method)
{
	this->_method = method;
}

void	HttpRequest::setPath(std::string path)
{
    this->_path = path;
}

void	HttpRequest::setQueryString(std::string query)
{
    this->_query_string = query;
}

void	HttpRequest::setVersion(std::string version)
{
	this->_version = version;
}

void	HttpRequest::setHeader(std::string key, std::string value)
{
	this->_header[key] = value;
}

void	HttpRequest::setBody(std::string body)
{
	this->_body = body;
}

void 	HttpRequest::setError(int errorCode)
{
	this->_error = errorCode;
}
void 	HttpRequest::setState(RequestState state)
{
	this->_state = state;
}

void 	HttpRequest::resetRequest()
{
	this->_method.clear();
	this->_path.clear();
	this->_query_string.clear();
	this->_version.clear();
	this->_header.clear();
	this->_body.clear();
	this->_error = 0;
	this->_state = INCOMPLETE;
}
