#ifndef	HIOCPdefs_H__
#define	HIOCPdefs_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014	 21:00
	file base:	HIOCPdefs
	author:		CHE
	
	purpose:	iocp公共定义
*********************************************************************/
#include "hglobal.h"
#include <winsock2.h>
#include <MSWSock.h>
CHE_NAMESPACE_BEGIN

typedef	enum tagIOCPOperatorType{
	//投递Accept操作
	Accept,
	//投递Send操作
	Send,
	//投递Recv操作
	Recv,
	//无意义value
	Null
}IOCPOperatorType;

// 缓冲区长度 (1024*8)
// 之所以为什么设置8K，也是一个江湖上的经验值
// 如果确实客户端发来的每组数据都比较少，那么就设置得小一些，省内存
#define	MAX_BUFFER_LEN		256
// 默认端口
#define	DEFAULT_PORT		12345

// 每一个处理器上产生多少个线程(为了最大限度的提升服务器性能，详见配套文档)
#define	WORKER_THREADS_PER_PROCESSOR 2
// 同时投递的Accept请求的数量(这个要根据实际的情况灵活设置)
#define	MAX_POST_ACCEPT				 10
// 传递给Worker线程的退出信号
#define	EXIT_CODE					 NULL

//////////////////
//句柄安全释放
inline void che_handle_del(HANDLE handle){
	if (handle != nullptr && handle != INVALID_HANDLE_VALUE){
		CloseHandle(handle);
	}
}
//////////////////
//句柄数组安全释放
inline void che_dyadic_handle_del(HANDLE *handle, int num) {
	if (handle == nullptr)	return;

	for (int i = 0; i < num; ++i) {
		che_handle_del(handle[i]);
	}
	delete[]handle;
}
//////////////////
//关闭socket
inline void closeSocket(SOCKET s) {
	s != INVALID_SOCKET ? closesocket(s) : che_noop();
}
CHE_NAMESPACE_END
#endif // HIOCPdefs_H__
