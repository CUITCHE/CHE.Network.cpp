#ifndef HttpPart_H__
#define HttpPart_H__

/********************************************************************
	created:	2014/12/09
	created:	9:12:2014   14:29
	file base:	HttpPart
	author:		CHE
	
	purpose:	一个http协议报头、主体
*********************************************************************/
#include "hglobal.h"
using std::string;
CHE_NAMESPACE_BEGIN
class HttpPart
{
	CHE_DISABLE_COPY(HttpPart);
	friend class HHttpRequest;
	friend class HHttpResponse;
public:
	HttpPart(){}

	~HttpPart();
	void setBody(const string &body);
	void setHeader(const string &headerName, const string &headerValue);
	void swap(HttpPart &other);
	bool operator!=(const HttpPart &rhs)const;
	bool operator==(const HttpPart &rhs)const;

private:
	string *header = nullptr;
	string *body = nullptr;
};
CHE_NAMESPACE_END
#endif // HttpPart_H__
