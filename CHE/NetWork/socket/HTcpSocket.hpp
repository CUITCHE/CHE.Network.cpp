#ifndef HTcpSocket_H__
#define HTcpSocket_H__
//
/********************************************************************
	created:	2014/10/12
	created:	12:10:2014   14:44
	file ext:	hpp
	author:		CHE
	
	purpose:	tcp socket
				调用本类需要在外部实现一个满足Recv_Tcp_Function定义的函数
				并通过HAbstractSocket::setRecvFunction(...)绑定，否则tcp
				接收到的数据无法返回回来
*********************************************************************/
#include "HAbstractSocket.h"
NAMESPACE_BEGIN_CHE
class HTcpSocket : public HAbstractSocket
{
public:
	HTcpSocket(HObject *parent = 0) :HAbstractSocket(che::TcpSocket, parent){}
	~HTcpSocket(){}
};
NAMESPACE_END_CHE
#endif // HTcpSocket_H__
