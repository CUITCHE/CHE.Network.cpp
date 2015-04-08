#ifndef HHttpRequest_H__
#define HHttpRequest_H__

/********************************************************************
	created:	2014/12/09
	created:	9:12:2014   15:46
	file base:	HHttpRequest
	author:		CHE
	
	purpose:	Http请求类
*********************************************************************/
#include "hglobal.h"
using std::string;
CHE_NAMESPACE_BEGIN
class HttpPart;
class HTcpSocket;
class HHttpRequest
{
	CHE_DISABLE_COPY(HHttpRequest);
public:
	//************************************
	// Method:    HHttpRequest
	// FullName:  che::HHttpRequest::HHttpRequest
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: const string & resourceURL 资源URL
	// Parameter: const string & hostURL 主机URL
	//************************************
	HHttpRequest(const string &resourceURL, const string &hostURL);
	//************************************
	// Method:    ~HHttpRequest
	// FullName:  che::HHttpRequest::~HHttpRequest
	// Access:    public 
	// Returns:   
	// Qualifier:
	//************************************
	~HHttpRequest();
	//************************************
	// Method:    setHttpVersion
	// FullName:  che::HHttpRequest::setHttpVersion
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: const string & version http协议版本号
	//************************************
	void setHttpVersion(const string &version);
private:
	string httpVersion = "HTTP/1.1";
	string resource;		//请求资源的路径
	HTcpSocket *tcp_scoket = nullptr;
	HttpPart *request_string = nullptr;
};
CHE_NAMESPACE_END
#endif // HHttpRequest_H__
