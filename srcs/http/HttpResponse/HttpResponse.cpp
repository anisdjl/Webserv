#include "../../../includes/http/HttpResponse.hpp"

HttpResponse::HttpResponse() : _state(NOT_BUILT),  _status_code(200), _status_message("OK"), _bytes_sent(0), _headers(), _body(""), _isDone(false) {}

HttpResponse::~HttpResponse(){};

std::string		HttpResponse::getResponse() const
{
	return this->_response;
}

std::string		HttpResponse::getBody() const
{
	return this->_body;
}

int         	HttpResponse::getStatusCode() const
{
	return this->_status_code;
}

ResponseState	HttpResponse::getState()
{
	return this->_state;
}

void			HttpResponse::setState(ResponseState state)
{
	this->_state = state;
}

void			HttpResponse::add_bytes_sent(unsigned int bytes)
{
	this->_bytes_sent += bytes;
}

unsigned int	HttpResponse::get_bytes_sent() const
{
	return this->_bytes_sent;
}

void			HttpResponse::setResponse(const std::string& response)
{
	this->_response = response;
}

void			HttpResponse::setBody(const std::string& body)
{
	this->_body = body;
}

void			HttpResponse::setisDone(bool state)
{
	this->_isDone = state;
}

bool			HttpResponse::getisDone()
{
	return this->_isDone;
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
