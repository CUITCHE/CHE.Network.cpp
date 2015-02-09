#ifndef IOContext_H__
#define IOContext_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014   21:02
	file base:	IOContext
	author:		CHE
	
	purpose:	单IO数据结构体定义(用于每一个重叠操作的参数)
*********************************************************************/
#include "HIOCPdefs.h"
NAMESPACE_BEGIN_CHE
class IOContext
{
public:
	// 初始化
	IOContext();
	// 释放掉Socket
	~IOContext();
	// 重置缓冲区内容
	void ResetBuffer(){ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);}

	char		m_szBuffer[MAX_BUFFER_LEN];		// 这个是WSABUF里具体存字符的缓冲区
	WSABUF		m_wsaBuf;						// WSA类型的缓冲区，用于给重叠操作传参数的
	SOCKET		m_sockAccept = INVALID_SOCKET;	// 这个网络操作所使用的Socket
	OVERLAPPED	m_Overlapped;					// 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)
	IOCPOperatorType m_OpType = Null;			// 标识网络操作的类型
};
NAMESPACE_END_CHE
#endif // IO_CONTEXT_H__
