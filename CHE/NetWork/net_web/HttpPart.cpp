#include "HttpPart.h"
CHE_NAMESPACE_BEGIN

HttpPart::~HttpPart()
{
	che_pointer_del(body, header);
}

void HttpPart::setBody(const string & _body)
{
	_memory_dynamic_crt(body);
	*this->body = _body;
}

void HttpPart::setHeader(const string & headerName, const string & headerValue)
{
	_memory_dynamic_crt(header);
	header->append(headerName).append(": ").append(headerValue).append("\r\n");
}

void HttpPart::swap(HttpPart & other)
{
	std::swap(body, other.body);
	std::swap(header, other.header);
}

bool HttpPart::operator!=(const HttpPart & rhs) const
{
	return !(*this==rhs);
}

bool HttpPart::operator==(const HttpPart & rhs) const
{
	return !body->compare(*rhs.body) && !header->compare(*rhs.header);
}

CHE_NAMESPACE_END
