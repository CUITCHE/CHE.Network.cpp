#include "HHttpRequest.h"
#include "HttpPart.h"
#include "HTcpSocket.hpp"
NAMESPACE_BEGIN_CHE

HHttpRequest::HHttpRequest(const string &resourceURL, const string &hostURL) 
:request_string(new HttpPart())
,resource(resourceURL)
{

}

HHttpRequest::~HHttpRequest()
{
	che_pointer_del(tcp_scoket, request_string);
}

void HHttpRequest::setHttpVersion(const string & version)
{
	httpVersion = version;
}

NAMESPACE_END_CHE