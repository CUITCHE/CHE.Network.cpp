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
CHE_NAMESPACE_BEGIN
class HUdpSocket : public HAbstractSocket
{
public:
	HUdpSocket(HObject *parent = 0) :HAbstractSocket(che::UdpSocket, parent){}
	~HUdpSocket(){}
	inline size_t writeDatagram(const char *data, size_t dataSize, const HHostAddress &address){ return HAbstractSocket::writeDatagram(data, dataSize, address); }
	inline size_t writeDatagram(const string &buffer, const HHostAddress &address){ return HAbstractSocket::writeDatagram(buffer.data(), buffer.size(), address); }

};
CHE_NAMESPACE_END
#endif // HUdpSocket_H__
