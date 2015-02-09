#ifndef HHttpResponse_H__
#define HHttpResponse_H__

/********************************************************************
	created:	2014/12/09
	created:	9:12:2014   19:14
	file base:	HHttpResponse
	author:		CHE
	
	purpose:	相应服务器请求
*********************************************************************/
#include "hglobal.h"
using std::string;
NAMESPACE_BEGIN_CHE
class HttpPart;
class HHttpResponse
{
	friend class HHttpRequest;
public:
	HHttpResponse();
	~HHttpResponse();

private:
	string status;								//状态行
	HttpPart *response_string = nullptr;		//报头head和正文body
};
NAMESPACE_END_CHE
#endif // HHttpResponse_H__
