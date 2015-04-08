#include "HHttpRequest.h"
#include "HttpPart.h"
#include "HTcpSocket.hpp"
CHE_NAMESPACE_BEGIN

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

CHE_NAMESPACE_END
