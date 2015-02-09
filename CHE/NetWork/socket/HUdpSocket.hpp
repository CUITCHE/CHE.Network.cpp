#ifndef HUdpSocket_H__
#define HUdpSocket_H__
//
/********************************************************************
	created:	2014/10/12
	created:	12:10:2014   14:49
	file ext:	hpp
	author:		CHE
	
	purpose:	udp socket
				调用本类需要在外部实现一个满足Recv_Udp_Function定义的函数
				并通过HAbstractSocket::setRecvFunction(...)绑定，否则udp
				接收到的数据无法返回回来
*********************************************************************/
#include "HAbstractSocket.h"
NAMESPACE_BEGIN_CHE
class HUdpSocket : public HAbstractSocket
{
public:
	HUdpSocket(HObject *parent = 0) :HAbstractSocket(che::UdpSocket, parent){}
	~HUdpSocket(){}
	H_INLINE size_t writeDatagram(const char *data, size_t dataSize, const HHostAddress &address){ return HAbstractSocket::writeDatagram(data, dataSize, address); }
	H_INLINE size_t writeDatagram(const string &buffer, const HHostAddress &address){ return HAbstractSocket::writeDatagram(buffer.data(), buffer.size(), address); }

};
NAMESPACE_END_CHE
#endif // HUdpSocket_H__
