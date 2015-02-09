#ifndef	HIOCP_H__
#define	HIOCP_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014	 20:19
	file base:	HIOCP
	author:		CHE

	purpose:	iocp模型，Windows NT 版本才可使用
				使用了CSDN用户PiggyXP的代码，参考：http://blog.csdn.net/piggyxp/article/details/6922277
				自己修改一部分
	*********************************************************************/
#ifndef	WIN32
#error 非win32平台不可使用iocp模型
#endif // !WIN
#include "SocketContext.h"
#include "HHostAddress.h"
#include <mutex>
#include <memory>
#include <thread>
#pragma	comment(lib,"ws2_32.lib")

using std::mutex;
using std::thread;
using std::unique_ptr;
using std::shared_ptr;

NAMESPACE_BEGIN_CHE

class IOContext;
class SocketContext;
class HighEffectIOBuffer;
struct HIOCPdata
{
	HIOCPdata()
		:m_address(new HHostAddress())
		, m_pListenContext(new SocketContext())
		, m_arrayClientContext(new list<SocketContext *>()){}
	~HIOCPdata(){
	//	che_dyadic_array_del(m_phWorkerThreads, m_nThreads);
		che_pointer_del(m_address);
		che_pointer_del(m_pListenContext);
		{
			auto iters = m_arrayClientContext->begin();
			auto itere = m_arrayClientContext->end();
			while (iters != itere) {
				auto pf = *iters;
				delete pf;
				++iters;
			}
			che_pointer_del(m_arrayClientContext);
		}
	}
	HANDLE m_hShutdownEvent = nullptr;		// 用来通知线程系统退出的事件，为了能够更好的退出线程
	HANDLE m_hIOCompletionPort = nullptr;	// 完成端口的句柄
	HANDLE* m_phWorkerThreads = nullptr;// 工作者线程的句柄指针
	int m_nThreads = 0;					// 生成的线程数量
	HHostAddress *m_address = nullptr;	//保存服务端的ip和端口信息
	mutex mtx;							// 用于Worker线程同步的互斥量

	list<SocketContext *>	*m_arrayClientContext;	// 客户端Socket的Context信息
	SocketContext *m_pListenContext;						// 用于监听的Socket的Context信息
	LPFN_ACCEPTEX m_lpfnAcceptEx;							// AcceptEx 和 GetAcceptExSockaddrs 的函数指针，用于调用这两个扩展函数
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockAddrs;
};
class HIOCP final
{
public:
	HIOCP(HHostAddress &&address, uint16 port = 0);
	~HIOCP();

	//启动服务
	bool start();

	//发送数据，将保证数据完整发送，否则返回SOCKET_ERROR或者小于len的数
	uint32 write(SOCKET s, const char *buffer, uint32 len);

	//停止服务
	void stop();

	// 获得本机的IP地址
	const char*	getLocalIP();

	//获得iocp的共享缓冲区引用
	unique_ptr<HighEffectIOBuffer>& buffer_block() { return shared_buffer_block; }
protected:
	// 加载Socket库
	bool loadSocketLib();

	// 卸载Socket库，彻底完事
	void unloadSocketLib() { WSACleanup(); }

	// 初始化IOCP
	bool initializeIOCP();

	// 初始化Socket
	bool initializeListenSocket();

	// 最后释放资源
	void delInitialize();

	// 投递Accept请求
	bool postAccept(IOContext *pAcceptIoContext);

	// 投递接收数据请求
	bool postRecv(IOContext *pIoContext);

	// 在有客户端连入的时候，进行处理
	bool doAccpet(SocketContext *pSocketContext, IOContext *pIoContext);

	// 在有接收的数据到达的时候，进行处理
	bool doRecv(SocketContext *pSocketContext, IOContext *pIoContext);

	// 将客户端的相关信息存储到数组中
	void addToContextList(SocketContext *pSocketContext);

	// 将客户端的信息从数组中移除
	void removeContext(SocketContext *pSocketContext);

	// 清空客户端信息
	void clearContextList();

	// 将句柄绑定到完成端口中
	bool associateWithIOCP(SocketContext *pContext);

	// 处理完成端口上的错误
	bool handleError(SocketContext *pContext, const DWORD& dwErr);

	// 线程函数，为IOCP请求服务的工作者线程
	static DWORD WINAPI workerThread(LPVOID lpParam);

	// 获得本机的处理器数量
	int	getNoOfProcessors();

	// 判断客户端Socket是否已经断开
	bool isSocketAlive(SOCKET s);
private:
	unique_ptr<HIOCPdata> d_ptr;
	unique_ptr<HighEffectIOBuffer> shared_buffer_block;
};
NAMESPACE_END_CHE
#endif // HIOCP_H__
