#ifndef SOCKET_CONTEXT_H__
#define SOCKET_CONTEXT_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014   21:09
	file base:	SocketContext
	author:		CHE
	
	purpose:	单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
*********************************************************************/
#include "HIOCPdefs.h"
#include <list>
using std::list;

CHE_NAMESPACE_BEGIN
class IOContext;
class SocketContext
{
public:
	// 初始化
	SocketContext();

	// 释放资源
	~SocketContext();

	// 获取一个新的IoContext
	IOContext* getOneIOContext();

	// 从数组中移除一个指定的IoContext
	void removeOneContext(IOContext *pContext);

	SOCKET		m_Socket = INVALID_SOCKET;			// 每一个客户端连接的Socket
	SOCKADDR_IN	m_ClientAddr;						// 客户端的地址
	list<IOContext*> m_arrayIoContext;	// 客户端网络操作的上下文数据，
	// 也就是说对于每一个客户端Socket，是可以在上面同时投递多个IO请求的

};
CHE_NAMESPACE_END
#endif // SOCKET_CONTEXT_H__
