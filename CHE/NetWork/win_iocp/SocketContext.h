#ifndef SOCKET_CONTEXT_H__
#define SOCKET_CONTEXT_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014   21:09
	file base:	SocketContext
	author:		CHE
	
	purpose:	��������ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)
*********************************************************************/
#include "HIOCPdefs.h"
#include <list>
using std::list;

NAMESPACE_BEGIN_CHE
class IOContext;
class SocketContext
{
public:
	// ��ʼ��
	SocketContext();

	// �ͷ���Դ
	~SocketContext();

	// ��ȡһ���µ�IoContext
	IOContext* getOneIOContext();

	// ���������Ƴ�һ��ָ����IoContext
	void removeOneContext(IOContext *pContext);

	SOCKET		m_Socket = INVALID_SOCKET;			// ÿһ���ͻ������ӵ�Socket
	SOCKADDR_IN	m_ClientAddr;						// �ͻ��˵ĵ�ַ
	list<IOContext*> m_arrayIoContext;	// �ͻ���������������������ݣ�
	// Ҳ����˵����ÿһ���ͻ���Socket���ǿ���������ͬʱͶ�ݶ��IO�����

};
NAMESPACE_END_CHE
#endif // SOCKET_CONTEXT_H__
