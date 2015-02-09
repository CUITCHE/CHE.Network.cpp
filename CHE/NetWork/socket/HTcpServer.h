#ifndef HTcpServer_H__
#define HTcpServer_H__
//
/********************************************************************
	created:	2014/10/12
	created:	12:10:2014   21:20
	file ext:	hpp
	author:		CHE

	purpose:	tcp server
	�������ɵ�HTcpSocketָ�룬HTcpServer�����ڴ����
	����shared_ptr�����ڴ�
	*********************************************************************/
#include "../NetWork/socket/private/HTcpServerPrivate.h"
#include "HTcpSocket.hpp"
NAMESPACE_BEGIN_CHE

class HTcpServer : public HThread
{
	CHE_DISABLE_COPY(HTcpServer)
	CHE_PRIVATE_DECLARE(HTcpServer)
public:
	template<typename TcpSocketCallbackFunc>
	HTcpServer(TcpSocketCallbackFunc func, HObject *parent = 0)
		:HThread(parent)
		, d_ptr(new HTcpServerPrivate())
	{
		Socket_Pending_Function functor = std::bind(func, std::placeholders::_1);
		HTcpServer_impl(move_quick(functor));
	}
	
	~HTcpServer() override;

	/*ֹͣ�������ر�socket���ͷ�socket��Դ*/
	void close();

	/*�������ɹ�����true*/
	bool listen(const HHostAddress &address, uint16 port = 0);

	bool listen(HHostAddress &&address, uint16 port = 0);

	void keep() { HThread::tryStop(); }
	/*Accept����������*/
	static int CALLBACK condtionFunc(LPWSABUF lpCallerId, LPWSABUF lpCallerData, LPQOS lpSQOS,
		LPQOS lpGQOS, LPWSABUF lpCalleeId, LPWSABUF lpCalleeData, GROUP FAR *g, DWORD_PTR dwCallbackData);

protected:
	/*����ѭ��accept����*/
	void cycle_accept_thread();

	static void GetPeerIpv4Address(const LPWSABUF buffer, HHostAddress &addr);

	bool listen_impl();

	void HTcpServer_impl(Socket_Pending_Function &&func);
private:
	unique_ptr<HTcpServerPrivate> d_ptr;
};
NAMESPACE_END_CHE
#endif // HTcpServer_H__