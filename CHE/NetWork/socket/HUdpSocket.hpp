#ifndef HUdpSocket_H__
#define HUdpSocket_H__
//
/********************************************************************
	created:	2014/10/12
	created:	12:10:2014   14:49
	file ext:	hpp
	author:		CHE
	
	purpose:	udp socket
				���ñ�����Ҫ���ⲿʵ��һ������Recv_Udp_Function����ĺ���
				��ͨ��HAbstractSocket::setRecvFunction(...)�󶨣�����udp
				���յ��������޷����ػ���
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
