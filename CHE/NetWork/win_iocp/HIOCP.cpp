#include "HIOCP.h"
#include "IOContext.h"
#include "HighEffectIOBuffer.h"
#include "HLog.h"
#include <cassert>
#include <algorithm>
#include <atomic>
CHE_NAMESPACE_BEGIN

HIOCP::HIOCP(HHostAddress &&address, uint16 port /*= 0*/) 
	:d_ptr(new HIOCPdata())
	,shared_buffer_block(new HighEffectIOBuffer())
{
	*d_ptr->m_address = move_quick(address);
	d_ptr->m_address->port(port);
}

HIOCP::~HIOCP()
{
	this->stop();
}

bool HIOCP::start()
{
	//加载socket资源
	this->loadSocketLib();

	d_ptr->m_hShutdownEvent	= CreateEvent(NULL,	TRUE, FALSE, NULL);

	//初始化IOCP
	if (false == initializeIOCP()){
		logger->warningLog( "iocp init	failed!\n");
		return false;
	}
	//初始化监听socket
	if (false == initializeListenSocket()){
		logger->warningLog("listen socket	init failed!\n");
		return false;
	}
	return true;
}

uint32 HIOCP::write(SOCKET s, const char *buffer, uint32 len)
{
	uint32 lenper = ::send(s, buffer, len, 0);

	if (lenper == SOCKET_ERROR) {
		return SOCKET_ERROR;
	}
	uint32 sendSum = lenper;
	while (sendSum < len) {
		lenper = ::send(s, buffer + sendSum, len - sendSum, 0);
		if (lenper == SOCKET_ERROR) {
			break;
		}
		sendSum += lenper;
	}
	return sendSum;
}

void HIOCP::stop()
{
	if (d_ptr->m_pListenContext	!= NULL	&& d_ptr->m_pListenContext->m_Socket !=	INVALID_SOCKET){
		//激活关闭消息通知
		SetEvent(d_ptr->m_hShutdownEvent);
		for	(int i = 0;	i <	d_ptr->m_nThreads; ++i){
			//通知所有的完成端口操作退出
			PostQueuedCompletionStatus(d_ptr->m_hIOCompletionPort, 0, (DWORD)EXIT_CODE,	NULL);
			//等待所有的客户端资源退出
			WaitForMultipleObjects(d_ptr->m_nThreads, d_ptr->m_phWorkerThreads,	TRUE, INFINITE);
			//清除客户列表信息
			this->clearContextList();
			//释放其他资源
			this->delInitialize();
		}
	}
	//清理socket资源
	this->unloadSocketLib();
}

const char*	HIOCP::getLocalIP()
{
	return d_ptr->m_address->toString();
}

bool HIOCP::loadSocketLib()
{
	WSADATA	wsaData;
	int	nResult;
	nResult	= WSAStartup(MAKEWORD(2, 2), &wsaData);
	// 错误(一般都不可能出现)
	if (NO_ERROR !=	nResult)
	{
		logger->warningLog("初始化WinSock 2.2失败！\n");
		return false;
	}

	return true;
}

bool HIOCP::initializeIOCP()
{
	//建立第一个完成端口
	d_ptr->m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL,	0, 0);

	if (NULL ==	d_ptr->m_hIOCompletionPort){
		logger->warningLog( "creation of complete port failed, code:", WSAGetLastError());
		return false;
	}
	d_ptr->m_nThreads =	WORKER_THREADS_PER_PROCESSOR * getNoOfProcessors();

	//为工作线程初始化句柄
	d_ptr->m_phWorkerThreads = new HANDLE[d_ptr->m_nThreads];

	//创建工作线程
	DWORD nThreadID;
	for	(int i = 0;	i <	d_ptr->m_nThreads; ++i){
		d_ptr->m_phWorkerThreads[i] = ::CreateThread(0, 0, workerThread, (void *)this, 0, &nThreadID);
	}
	return true;
}

bool HIOCP::initializeListenSocket()
{
	// AcceptEx	和 GetAcceptExSockaddrs 的GUID，用于导出函数指针
	GUID GuidAcceptEx =	WSAID_ACCEPTEX;
	GUID GuidGetAcceptExSockAddrs =	WSAID_GETACCEPTEXSOCKADDRS;

	//生成用于监听的socket的信息
	d_ptr->m_pListenContext	= new SocketContext();

	d_ptr->m_pListenContext->m_Socket =	WSASocketW(AF_INET, SOCK_STREAM,	0, NULL, 0,	WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == d_ptr->m_pListenContext->m_Socket){
		logger->warningLog( "listen socket	init failed!,code:", WSAGetLastError());
		return false;
	}

	//将listen socket 绑定至完成端口中
	if (NULL ==	CreateIoCompletionPort(
		(HANDLE)d_ptr->m_pListenContext->m_Socket
		, d_ptr->m_hIOCompletionPort
		, (DWORD)d_ptr->m_pListenContext
		, 0)
		){
		logger->warningLog( "bind the listen socket failed!,code:", WSAGetLastError());
		return false;
	}

	//绑定地址和端口
	if (SOCKET_ERROR ==	bind(d_ptr->m_pListenContext->m_Socket,	d_ptr->m_address->sockAddr(), sizeof(sockaddr_in))){
		logger->warningLog( "bind exectue failed!, code:", WSAGetLastError());
		return false;
	}

	//监听
	if (SOCKET_ERROR ==	listen(d_ptr->m_pListenContext->m_Socket, SOMAXCONN)){
		logger->warningLog( "listen() failed, code:", WSAGetLastError());
		return false;
	}

	// 使用AcceptEx函数，因为这个是属于WinSock2规范之外的微软另外提供的扩展函数
	// 所以需要额外获取一下函数的指针，
	// 获取AcceptEx函数指针
	DWORD dwBytes =	0;
	if (SOCKET_ERROR ==	WSAIoctl(
		d_ptr->m_pListenContext->m_Socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx,
		sizeof(GuidAcceptEx),
		&d_ptr->m_lpfnAcceptEx,
		sizeof(d_ptr->m_lpfnAcceptEx),
		&dwBytes,
		NULL,
		NULL)){
		logger->warningLog( "WSAIoctl 未能获取AcceptEx函数指针。错误代码:", WSAGetLastError());
		this->delInitialize();
		return false;
	}

	// 获取GetAcceptExSockAddrs函数指针，也是同理
	if (SOCKET_ERROR ==	WSAIoctl(
		d_ptr->m_pListenContext->m_Socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidGetAcceptExSockAddrs,
		sizeof(GuidGetAcceptExSockAddrs),
		&d_ptr->m_lpfnGetAcceptExSockAddrs,
		sizeof(d_ptr->m_lpfnGetAcceptExSockAddrs),
		&dwBytes,
		NULL,
		NULL)){
		logger->warningLog( "WSAIoctl 未能获取GuidGetAcceptExSockAddrs函数指针。错误代码:", WSAGetLastError());
		this->delInitialize();
		return false;
	}
	// 为AcceptEx 准备参数，然后投递AcceptEx I/O请求
	for	(int i = 0;	i <	MAX_POST_ACCEPT; i++){
		// 新建一个IO_CONTEXT
		auto pAcceptIoContext =	d_ptr->m_pListenContext->getOneIOContext();

		if (false == this->postAccept(pAcceptIoContext)){
			d_ptr->m_pListenContext->removeOneContext(pAcceptIoContext);
			return false;
		}
	}

	return true;
}

void HIOCP::delInitialize()
{
	che_handle_del(d_ptr->m_hShutdownEvent);

	//清理工作线程
	che_dyadic_handle_del(d_ptr->m_phWorkerThreads, d_ptr->m_nThreads);

	//关闭ICOP句柄
	che_handle_del(d_ptr->m_hIOCompletionPort);

	//关闭监听socket
	che_pointer_del(d_ptr->m_pListenContext);

	logger->warningLog( "resource delete over!");
}

bool HIOCP::postAccept(IOContext *pAcceptIoContext)
{
	assert(INVALID_SOCKET != d_ptr->m_pListenContext->m_Socket);
	DWORD dwBytes = 0;
	pAcceptIoContext->m_OpType = Accept;
	WSABUF *p_wbuf = &pAcceptIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pAcceptIoContext->m_Overlapped;
	// 为以后新连入的客户端先准备好Socket( 这个是与传统accept最大的区别 ) 
	pAcceptIoContext->m_sockAccept = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == pAcceptIoContext->m_sockAccept){
		logger->warningLog( "创建用于Accept的Socket失败！错误代码:", WSAGetLastError());
		return false;
	}

	// 投递AcceptEx
	if (FALSE == d_ptr->m_lpfnAcceptEx(
		d_ptr->m_pListenContext->m_Socket
		, pAcceptIoContext->m_sockAccept
		, p_wbuf->buf
		, p_wbuf->len -	((sizeof(SOCKADDR_IN) +	16)	* 2)
		, sizeof(SOCKADDR_IN) +	16,	sizeof(SOCKADDR_IN)	+ 16, &dwBytes,	p_ol)
		){
		if (WSA_IO_PENDING != WSAGetLastError()){
			logger->warningLog( "投递 AcceptEx 请求失败，错误代码:", WSAGetLastError());
			return false;
		}
	}

	return true;
}

bool HIOCP::postRecv(IOContext *pIoContext)
{
	// 初始化变量
	DWORD dwFlags =	0;
	DWORD dwBytes =	0;
	WSABUF *p_wbuf = &pIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pIoContext->m_Overlapped;

	pIoContext->ResetBuffer();
	pIoContext->m_OpType = Recv;

	// 初始化完成后，，投递WSARecv请求
	int	nBytesRecv = WSARecv(pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, &dwFlags, p_ol, NULL);

	// 如果返回值错误，并且错误的代码并非是Pending的话，那就说明这个重叠请求失败了
	if ((SOCKET_ERROR == nBytesRecv) &&	(WSA_IO_PENDING	!= WSAGetLastError())){
		logger->warningLog( "post one WSARecv faild！");
		return false;
	}
	return true;
}

bool HIOCP::doAccpet(SocketContext *pSocketContext, IOContext *pIoContext)
{
	SOCKADDR_IN* ClientAddr = NULL;
	SOCKADDR_IN* LocalAddr = NULL;
	int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);

	///////////////////////////////////////////////////////////////////////////
	// 1. 首先取得连入客户端的地址信息
	// 这个 m_lpfnGetAcceptExSockAddrs 不得了啊~~~~~~
	// 不但可以取得客户端和本地端的地址信息，还能顺便取出客户端发来的第一组数据，老强大了...
	d_ptr->m_lpfnGetAcceptExSockAddrs(pIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);

//	static HHostAddress addr;
//	memcpy((sockaddr_in*)addr.sockAddr(), ClientAddr, sizeof(ClientAddr));
//	fprintf_s(stderr, "客户额 %s:%d 信息：%s.\n", addr.toString(), addr.port(), pIoContext->m_wsaBuf.buf);
	NetIOBuffer tmp;
	shared_buffer_block->buffer_unused(&tmp);
	tmp.s = pIoContext->m_sockAccept;
	memcpy(tmp.buf(), pIoContext->m_wsaBuf.buf, sizeof(char)*MAX_BUFFER_LEN);
	shared_buffer_block->buffer_unused(move_quick(tmp));

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. 这里需要注意，这里传入的这个是ListenSocket上的Context，这个Context我们还需要用于监听下一个连接
	// 所以我还得要将ListenSocket上的Context复制出来一份为新连入的Socket新建一个SocketContext

	SocketContext *pNewSocketContext(new SocketContext);
	pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
	memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));

	// 参数设置完毕，将这个Socket和完成端口绑定(这也是一个关键步骤)
	if (false == this->associateWithIOCP(pNewSocketContext)){
		che_pointer_del(pNewSocketContext);
		goto ret;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 3. 继续，建立其下的IoContext，用于在这个Socket上投递第一个Recv数据请求
	auto pNewIoContext = pNewSocketContext->getOneIOContext();
	pNewIoContext->m_OpType = Recv;
	pNewIoContext->m_sockAccept = pNewSocketContext->m_Socket;
	// 如果Buffer需要保留，就自己拷贝一份出来
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	// 绑定完毕之后，就可以开始在这个Socket上投递完成请求了
	if (false == this->postRecv(pNewIoContext)){
		pNewSocketContext->removeOneContext(pNewIoContext);
		goto ret;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 4. 如果投递成功，那么就把这个有效的客户端信息，加入到ContextList中去(需要统一管理，方便释放资源)
	this->addToContextList(pNewSocketContext);

	//4.1 将成功Accept的socket放入save 容器中，提供给接口
	// TODO：


	////////////////////////////////////////////////////////////////////////////////////////////////
	// 5. 使用完毕之后，把Listen Socket的那个IoContext重置，然后准备投递新的AcceptEx
ret:
	pIoContext->ResetBuffer();
	return this->postAccept(pIoContext);
}

bool HIOCP::doRecv(SocketContext *pSocketContext, IOContext *pIoContext)
{
	// 先把上一次的数据显示出现，然后就重置状态，发出下一个Recv请求
	SOCKADDR_IN* ClientAddr = &pSocketContext->m_ClientAddr;
//	static HHostAddress addr;
//	memcpy((sockaddr_in*)addr.sockAddr(), ClientAddr, sizeof(ClientAddr));
//	fprintf_s(stderr, "客户端 %s:%d 信息：%s.\n", addr.toString(), addr.port(), pIoContext->m_wsaBuf.buf);
	//把buffer拷贝出来，抛给上层
	//TODO：
	NetIOBuffer buffer;
	shared_buffer_block->buffer_unused(&buffer);
	memcpy(buffer.buf(), pIoContext->m_wsaBuf.buf, sizeof(char)*MAX_BUFFER_LEN);
	buffer.s = pIoContext->m_sockAccept;
	shared_buffer_block->buffer_unused(move_quick(buffer));

	// 然后开始投递下一个WSARecv请求
	return postRecv(pIoContext);
}

void HIOCP::addToContextList(SocketContext *pSocketContext)
{
	std::lock_guard<mutex> locker(d_ptr->mtx);
	d_ptr->m_arrayClientContext->push_back(pSocketContext);

}

void HIOCP::removeContext(SocketContext *pSocketContext)
{
	static HHostAddress addr;
	addr.setAddress(&pSocketContext->m_ClientAddr);
	logger->warningLog( "有客户端退出：", addr.toString(), ":", addr.port());
	{
		std::lock_guard<mutex> locker(d_ptr->mtx);
		std::remove(d_ptr->m_arrayClientContext->begin(), d_ptr->m_arrayClientContext->end(), pSocketContext);
	}
	delete pSocketContext;
}

void HIOCP::clearContextList()
{
	std::lock_guard<mutex> locker(d_ptr->mtx);
	d_ptr->m_arrayClientContext->clear();
}

bool HIOCP::associateWithIOCP(SocketContext *pContext)
{
	// 将用于和客户端通信的SOCKET绑定到完成端口中
		HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, d_ptr->m_hIOCompletionPort, (DWORD)pContext, 0);

	if (NULL ==	hTemp){
		logger->warningLog( "执行CreateIoCompletionPort()出现错误.错误代码：", GetLastError());
		return false;
	}

	return true;
}

bool HIOCP::handleError(SocketContext *pContext, const DWORD& dwErr)
{
	// 如果是超时了，就再继续等吧  
	if (WAIT_TIMEOUT ==	dwErr){
		// 确认客户端是否还活着...
		if (!isSocketAlive(pContext->m_Socket)){
			logger->warningLog( "检测到客户端异常退出！");
			this->removeContext(pContext);
			return true;
		}
		else{
			logger->warningLog( "网络操作超时！重试中...");
			return true;
		}
	}
	else if	(ERROR_NETNAME_DELETED == dwErr){
		// 可能是客户端异常退出了
		logger->warningLog( "检测到客户端异常退出！");
		this->removeContext(pContext);
		return true;
	}
	else{
		logger->warningLog( "完成端口操作出现错误，线程退出。错误代码：", dwErr);
		return false;
	}
}

DWORD HIOCP::workerThread(LPVOID lpParam)
{
	OVERLAPPED			 *pOverlapped =	nullptr;
	SocketContext	*pSocketContext = nullptr;
	DWORD				 dwBytesTransfered = 0;
	auto pthis = (HIOCP *)lpParam;
	auto &d_ptr = pthis->d_ptr;
	// 循环处理请求，知道接收到Shutdown信息为止
	while (WAIT_OBJECT_0 !=	WaitForSingleObject(d_ptr->m_hShutdownEvent, 0)){
		BOOL bReturn = GetQueuedCompletionStatus(
			d_ptr->m_hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)(&pSocketContext),
			&pOverlapped,
			INFINITE);

		// 如果收到的是退出标志，则直接退出
		if (EXIT_CODE == (DWORD)(pSocketContext)){
			break;
		}

		// 判断是否出现了错误
		if (!bReturn){
			DWORD dwErr	= GetLastError();
			// 显示一下提示信息
			if (!pthis->handleError(pSocketContext, dwErr)){
				break;
			}
			continue;
		}
		else{
			// 读取传入的参数
			IOContext* pIoContext = CONTAINING_RECORD(pOverlapped, IOContext, m_Overlapped);

			// 判断是否有客户端断开了
			if ((0 == dwBytesTransfered) &&	(Recv == pIoContext->m_OpType || Send == pIoContext->m_OpType)){
			//	logger->warningLog("客户端 %:% 断开连接.", inet_ntoa(pSocketContext->m_ClientAddr.sin_addr),	ntohs(pSocketContext->m_ClientAddr.sin_port));
				// 释放掉对应的资源
				pthis->removeContext(pSocketContext);

				continue;
			}
			else{
				switch (pIoContext->m_OpType){
				
				// Accept  
				case Accept:{
					pthis->doAccpet(pSocketContext, pIoContext);
				}
				break;

				// RECV
				case Recv:{
					pthis->doRecv(pSocketContext, pIoContext);
				}
				break;
				// SEND
				case Send:{
					logger->warningLog( "没有send");
				}
				break;
				default:
					logger->warningLog( "_WorkThread中的	pIoContext->m_OpType 参数异常.");
					break;
				} //switch
			}//if
		}//if

	}//while

	logger->warningLog( "工作者线程退出.");
	return 0;
}

int	HIOCP::getNoOfProcessors()
{
	SYSTEM_INFO	si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

bool HIOCP::isSocketAlive(SOCKET s)
{
	int	nByteSent =	send(s,	"",	0, 0);
	if (-1 == nByteSent) return	false;
	return true;
}

CHE_NAMESPACE_END
