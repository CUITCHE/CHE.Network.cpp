#ifndef IOContext_H__
#define IOContext_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014   21:02
	file base:	IOContext
	author:		CHE
	
	purpose:	��IO���ݽṹ�嶨��(����ÿһ���ص������Ĳ���)
*********************************************************************/
#include "HIOCPdefs.h"
CHE_NAMESPACE_BEGIN
class IOContext
{
public:
	// ��ʼ��
	IOContext();
	// �ͷŵ�Socket
	~IOContext();
	// ���û���������
	void ResetBuffer(){ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);}

	char		m_szBuffer[MAX_BUFFER_LEN];		// �����WSABUF�������ַ��Ļ�����
	WSABUF		m_wsaBuf;						// WSA���͵Ļ����������ڸ��ص�������������
	SOCKET		m_sockAccept = INVALID_SOCKET;	// ������������ʹ�õ�Socket
	OVERLAPPED	m_Overlapped;					// ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)
	IOCPOperatorType m_OpType = Null;			// ��ʶ�������������
};
CHE_NAMESPACE_END
#endif // IO_CONTEXT_H__
