#ifndef HTcpSocket_H__
#define HTcpSocket_H__
//
/********************************************************************
	created:	2014/10/12
	created:	12:10:2014   14:44
	file ext:	hpp
	author:		CHE
	
	purpose:	tcp socket
				���ñ�����Ҫ���ⲿʵ��һ������Recv_Tcp_Function����ĺ���
				��ͨ��HAbstractSocket::setRecvFunction(...)�󶨣�����tcp
				���յ��������޷����ػ���
*********************************************************************/
#include "HAbstractSocket.h"
CHE_NAMESPACE_BEGIN
class HTcpSocket : public HAbstractSocket
{
public:
	HTcpSocket(HObject *parent = 0) :HAbstractSocket(che::TcpSocket, parent){}
	~HTcpSocket(){}
};
CHE_NAMESPACE_END
#endif // HTcpSocket_H__
