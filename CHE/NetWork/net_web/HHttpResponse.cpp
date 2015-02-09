#include "HHttpResponse.h"
#include "HttpPart.h"
NAMESPACE_BEGIN_CHE



HHttpResponse::HHttpResponse()
{

}

HHttpResponse::~HHttpResponse()
{
	che_pointer_del(response_string);
}

NAMESPACE_END_CHE