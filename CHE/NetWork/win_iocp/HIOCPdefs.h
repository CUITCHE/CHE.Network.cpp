#ifndef	HIOCPdefs_H__
#define	HIOCPdefs_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014	 21:00
	file base:	HIOCPdefs
	author:		CHE
	
	purpose:	iocp��������
*********************************************************************/
#include "hglobal.h"
#include <winsock2.h>
#include <MSWSock.h>
CHE_NAMESPACE_BEGIN

typedef	enum tagIOCPOperatorType{
	//Ͷ��Accept����
	Accept,
	//Ͷ��Send����
	Send,
	//Ͷ��Recv����
	Recv,
	//������value
	Null
}IOCPOperatorType;

// ���������� (1024*8)
// ֮����Ϊʲô����8K��Ҳ��һ�������ϵľ���ֵ
// ���ȷʵ�ͻ��˷�����ÿ�����ݶ��Ƚ��٣���ô�����õ�СһЩ��ʡ�ڴ�
#define	MAX_BUFFER_LEN		256
// Ĭ�϶˿�
#define	DEFAULT_PORT		12345

// ÿһ���������ϲ������ٸ��߳�(Ϊ������޶ȵ��������������ܣ���������ĵ�)
#define	WORKER_THREADS_PER_PROCESSOR 2
// ͬʱͶ�ݵ�Accept���������(���Ҫ����ʵ�ʵ�����������)
#define	MAX_POST_ACCEPT				 10
// ���ݸ�Worker�̵߳��˳��ź�
#define	EXIT_CODE					 NULL

//////////////////
//�����ȫ�ͷ�
inline void che_handle_del(HANDLE handle){
	if (handle != nullptr && handle != INVALID_HANDLE_VALUE){
		CloseHandle(handle);
	}
}
//////////////////
//������鰲ȫ�ͷ�
inline void che_dyadic_handle_del(HANDLE *handle, int num) {
	if (handle == nullptr)	return;

	for (int i = 0; i < num; ++i) {
		che_handle_del(handle[i]);
	}
	delete[]handle;
}
//////////////////
//�ر�socket
inline void closeSocket(SOCKET s) {
	s != INVALID_SOCKET ? closesocket(s) : che_noop();
}
CHE_NAMESPACE_END
#endif // HIOCPdefs_H__
