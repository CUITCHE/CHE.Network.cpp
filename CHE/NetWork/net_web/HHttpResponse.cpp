#include "HHttpResponse.h"
#include "HttpPart.h"
CHE_NAMESPACE_BEGIN



HHttpResponse::HHttpResponse()
{

}

HHttpResponse::~HHttpResponse()
{
	che_pointer_del(response_string);
}

CHE_NAMESPACE_END
