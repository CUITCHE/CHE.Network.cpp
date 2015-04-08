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
	//����socket��Դ
	this->loadSocketLib();

	d_ptr->m_hShutdownEvent	= CreateEvent(NULL,	TRUE, FALSE, NULL);

	//��ʼ��IOCP
	if (false == initializeIOCP()){
		logger->warningLog( "iocp init	failed!\n");
		return false;
	}
	//��ʼ������socket
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
		//����ر���Ϣ֪ͨ
		SetEvent(d_ptr->m_hShutdownEvent);
		for	(int i = 0;	i <	d_ptr->m_nThreads; ++i){
			//֪ͨ���е���ɶ˿ڲ����˳�
			PostQueuedCompletionStatus(d_ptr->m_hIOCompletionPort, 0, (DWORD)EXIT_CODE,	NULL);
			//�ȴ����еĿͻ�����Դ�˳�
			WaitForMultipleObjects(d_ptr->m_nThreads, d_ptr->m_phWorkerThreads,	TRUE, INFINITE);
			//����ͻ��б���Ϣ
			this->clearContextList();
			//�ͷ�������Դ
			this->delInitialize();
		}
	}
	//����socket��Դ
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
	// ����(һ�㶼�����ܳ���)
	if (NO_ERROR !=	nResult)
	{
		logger->warningLog("��ʼ��WinSock 2.2ʧ�ܣ�\n");
		return false;
	}

	return true;
}

bool HIOCP::initializeIOCP()
{
	//������һ����ɶ˿�
	d_ptr->m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL,	0, 0);

	if (NULL ==	d_ptr->m_hIOCompletionPort){
		logger->warningLog( "creation of complete port failed, code:", WSAGetLastError());
		return false;
	}
	d_ptr->m_nThreads =	WORKER_THREADS_PER_PROCESSOR * getNoOfProcessors();

	//Ϊ�����̳߳�ʼ�����
	d_ptr->m_phWorkerThreads = new HANDLE[d_ptr->m_nThreads];

	//���������߳�
	DWORD nThreadID;
	for	(int i = 0;	i <	d_ptr->m_nThreads; ++i){
		d_ptr->m_phWorkerThreads[i] = ::CreateThread(0, 0, workerThread, (void *)this, 0, &nThreadID);
	}
	return true;
}

bool HIOCP::initializeListenSocket()
{
	// AcceptEx	�� GetAcceptExSockaddrs ��GUID�����ڵ�������ָ��
	GUID GuidAcceptEx =	WSAID_ACCEPTEX;
	GUID GuidGetAcceptExSockAddrs =	WSAID_GETACCEPTEXSOCKADDRS;

	//�������ڼ�����socket����Ϣ
	d_ptr->m_pListenContext	= new SocketContext();

	d_ptr->m_pListenContext->m_Socket =	WSASocketW(AF_INET, SOCK_STREAM,	0, NULL, 0,	WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == d_ptr->m_pListenContext->m_Socket){
		logger->warningLog( "listen socket	init failed!,code:", WSAGetLastError());
		return false;
	}

	//��listen socket ������ɶ˿���
	if (NULL ==	CreateIoCompletionPort(
		(HANDLE)d_ptr->m_pListenContext->m_Socket
		, d_ptr->m_hIOCompletionPort
		, (DWORD)d_ptr->m_pListenContext
		, 0)
		){
		logger->warningLog( "bind the listen socket failed!,code:", WSAGetLastError());
		return false;
	}

	//�󶨵�ַ�Ͷ˿�
	if (SOCKET_ERROR ==	bind(d_ptr->m_pListenContext->m_Socket,	d_ptr->m_address->sockAddr(), sizeof(sockaddr_in))){
		logger->warningLog( "bind exectue failed!, code:", WSAGetLastError());
		return false;
	}

	//����
	if (SOCKET_ERROR ==	listen(d_ptr->m_pListenContext->m_Socket, SOMAXCONN)){
		logger->warningLog( "listen() failed, code:", WSAGetLastError());
		return false;
	}

	// ʹ��AcceptEx��������Ϊ���������WinSock2�淶֮���΢�������ṩ����չ����
	// ������Ҫ�����ȡһ�º�����ָ�룬
	// ��ȡAcceptEx����ָ��
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
		logger->warningLog( "WSAIoctl δ�ܻ�ȡAcceptEx����ָ�롣�������:", WSAGetLastError());
		this->delInitialize();
		return false;
	}

	// ��ȡGetAcceptExSockAddrs����ָ�룬Ҳ��ͬ��
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
		logger->warningLog( "WSAIoctl δ�ܻ�ȡGuidGetAcceptExSockAddrs����ָ�롣�������:", WSAGetLastError());
		this->delInitialize();
		return false;
	}
	// ΪAcceptEx ׼��������Ȼ��Ͷ��AcceptEx I/O����
	for	(int i = 0;	i <	MAX_POST_ACCEPT; i++){
		// �½�һ��IO_CONTEXT
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

	//�������߳�
	che_dyadic_handle_del(d_ptr->m_phWorkerThreads, d_ptr->m_nThreads);

	//�ر�ICOP���
	che_handle_del(d_ptr->m_hIOCompletionPort);

	//�رռ���socket
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
	// Ϊ�Ժ�������Ŀͻ�����׼����Socket( ������봫ͳaccept�������� ) 
	pAcceptIoContext->m_sockAccept = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == pAcceptIoContext->m_sockAccept){
		logger->warningLog( "��������Accept��Socketʧ�ܣ��������:", WSAGetLastError());
		return false;
	}

	// Ͷ��AcceptEx
	if (FALSE == d_ptr->m_lpfnAcceptEx(
		d_ptr->m_pListenContext->m_Socket
		, pAcceptIoContext->m_sockAccept
		, p_wbuf->buf
		, p_wbuf->len -	((sizeof(SOCKADDR_IN) +	16)	* 2)
		, sizeof(SOCKADDR_IN) +	16,	sizeof(SOCKADDR_IN)	+ 16, &dwBytes,	p_ol)
		){
		if (WSA_IO_PENDING != WSAGetLastError()){
			logger->warningLog( "Ͷ�� AcceptEx ����ʧ�ܣ��������:", WSAGetLastError());
			return false;
		}
	}

	return true;
}

bool HIOCP::postRecv(IOContext *pIoContext)
{
	// ��ʼ������
	DWORD dwFlags =	0;
	DWORD dwBytes =	0;
	WSABUF *p_wbuf = &pIoContext->m_wsaBuf;
	OVERLAPPED *p_ol = &pIoContext->m_Overlapped;

	pIoContext->ResetBuffer();
	pIoContext->m_OpType = Recv;

	// ��ʼ����ɺ󣬣�Ͷ��WSARecv����
	int	nBytesRecv = WSARecv(pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, &dwFlags, p_ol, NULL);

	// �������ֵ���󣬲��Ҵ���Ĵ��벢����Pending�Ļ����Ǿ�˵������ص�����ʧ����
	if ((SOCKET_ERROR == nBytesRecv) &&	(WSA_IO_PENDING	!= WSAGetLastError())){
		logger->warningLog( "post one WSARecv faild��");
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
	// 1. ����ȡ������ͻ��˵ĵ�ַ��Ϣ
	// ��� m_lpfnGetAcceptExSockAddrs �����˰�~~~~~~
	// ��������ȡ�ÿͻ��˺ͱ��ض˵ĵ�ַ��Ϣ������˳��ȡ���ͻ��˷����ĵ�һ�����ݣ���ǿ����...
	d_ptr->m_lpfnGetAcceptExSockAddrs(pIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);

//	static HHostAddress addr;
//	memcpy((sockaddr_in*)addr.sockAddr(), ClientAddr, sizeof(ClientAddr));
//	fprintf_s(stderr, "�ͻ��� %s:%d ��Ϣ��%s.\n", addr.toString(), addr.port(), pIoContext->m_wsaBuf.buf);
	NetIOBuffer tmp;
	shared_buffer_block->buffer_unused(&tmp);
	tmp.s = pIoContext->m_sockAccept;
	memcpy(tmp.buf(), pIoContext->m_wsaBuf.buf, sizeof(char)*MAX_BUFFER_LEN);
	shared_buffer_block->buffer_unused(move_quick(tmp));

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. ������Ҫע�⣬���ﴫ��������ListenSocket�ϵ�Context�����Context���ǻ���Ҫ���ڼ�����һ������
	// �����һ���Ҫ��ListenSocket�ϵ�Context���Ƴ���һ��Ϊ�������Socket�½�һ��SocketContext

	SocketContext *pNewSocketContext(new SocketContext);
	pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
	memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));

	// ����������ϣ������Socket����ɶ˿ڰ�(��Ҳ��һ���ؼ�����)
	if (false == this->associateWithIOCP(pNewSocketContext)){
		che_pointer_del(pNewSocketContext);
		goto ret;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 3. �������������µ�IoContext�����������Socket��Ͷ�ݵ�һ��Recv��������
	auto pNewIoContext = pNewSocketContext->getOneIOContext();
	pNewIoContext->m_OpType = Recv;
	pNewIoContext->m_sockAccept = pNewSocketContext->m_Socket;
	// ���Buffer��Ҫ���������Լ�����һ�ݳ���
	//memcpy( pNewIoContext->m_szBuffer,pIoContext->m_szBuffer,MAX_BUFFER_LEN );

	// �����֮�󣬾Ϳ��Կ�ʼ�����Socket��Ͷ�����������
	if (false == this->postRecv(pNewIoContext)){
		pNewSocketContext->removeOneContext(pNewIoContext);
		goto ret;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 4. ���Ͷ�ݳɹ�����ô�Ͱ������Ч�Ŀͻ�����Ϣ�����뵽ContextList��ȥ(��Ҫͳһ���������ͷ���Դ)
	this->addToContextList(pNewSocketContext);

	//4.1 ���ɹ�Accept��socket����save �����У��ṩ���ӿ�
	// TODO��


	////////////////////////////////////////////////////////////////////////////////////////////////
	// 5. ʹ�����֮�󣬰�Listen Socket���Ǹ�IoContext���ã�Ȼ��׼��Ͷ���µ�AcceptEx
ret:
	pIoContext->ResetBuffer();
	return this->postAccept(pIoContext);
}

bool HIOCP::doRecv(SocketContext *pSocketContext, IOContext *pIoContext)
{
	// �Ȱ���һ�ε�������ʾ���֣�Ȼ�������״̬��������һ��Recv����
	SOCKADDR_IN* ClientAddr = &pSocketContext->m_ClientAddr;
//	static HHostAddress addr;
//	memcpy((sockaddr_in*)addr.sockAddr(), ClientAddr, sizeof(ClientAddr));
//	fprintf_s(stderr, "�ͻ��� %s:%d ��Ϣ��%s.\n", addr.toString(), addr.port(), pIoContext->m_wsaBuf.buf);
	//��buffer�����������׸��ϲ�
	//TODO��
	NetIOBuffer buffer;
	shared_buffer_block->buffer_unused(&buffer);
	memcpy(buffer.buf(), pIoContext->m_wsaBuf.buf, sizeof(char)*MAX_BUFFER_LEN);
	buffer.s = pIoContext->m_sockAccept;
	shared_buffer_block->buffer_unused(move_quick(buffer));

	// Ȼ��ʼͶ����һ��WSARecv����
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
	logger->warningLog( "�пͻ����˳���", addr.toString(), ":", addr.port());
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
	// �����ںͿͻ���ͨ�ŵ�SOCKET�󶨵���ɶ˿���
		HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, d_ptr->m_hIOCompletionPort, (DWORD)pContext, 0);

	if (NULL ==	hTemp){
		logger->warningLog( "ִ��CreateIoCompletionPort()���ִ���.������룺", GetLastError());
		return false;
	}

	return true;
}

bool HIOCP::handleError(SocketContext *pContext, const DWORD& dwErr)
{
	// ����ǳ�ʱ�ˣ����ټ����Ȱ�  
	if (WAIT_TIMEOUT ==	dwErr){
		// ȷ�Ͽͻ����Ƿ񻹻���...
		if (!isSocketAlive(pContext->m_Socket)){
			logger->warningLog( "��⵽�ͻ����쳣�˳���");
			this->removeContext(pContext);
			return true;
		}
		else{
			logger->warningLog( "���������ʱ��������...");
			return true;
		}
	}
	else if	(ERROR_NETNAME_DELETED == dwErr){
		// �����ǿͻ����쳣�˳���
		logger->warningLog( "��⵽�ͻ����쳣�˳���");
		this->removeContext(pContext);
		return true;
	}
	else{
		logger->warningLog( "��ɶ˿ڲ������ִ����߳��˳���������룺", dwErr);
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
	// ѭ����������֪�����յ�Shutdown��ϢΪֹ
	while (WAIT_OBJECT_0 !=	WaitForSingleObject(d_ptr->m_hShutdownEvent, 0)){
		BOOL bReturn = GetQueuedCompletionStatus(
			d_ptr->m_hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)(&pSocketContext),
			&pOverlapped,
			INFINITE);

		// ����յ������˳���־����ֱ���˳�
		if (EXIT_CODE == (DWORD)(pSocketContext)){
			break;
		}

		// �ж��Ƿ�����˴���
		if (!bReturn){
			DWORD dwErr	= GetLastError();
			// ��ʾһ����ʾ��Ϣ
			if (!pthis->handleError(pSocketContext, dwErr)){
				break;
			}
			continue;
		}
		else{
			// ��ȡ����Ĳ���
			IOContext* pIoContext = CONTAINING_RECORD(pOverlapped, IOContext, m_Overlapped);

			// �ж��Ƿ��пͻ��˶Ͽ���
			if ((0 == dwBytesTransfered) &&	(Recv == pIoContext->m_OpType || Send == pIoContext->m_OpType)){
			//	logger->warningLog("�ͻ��� %:% �Ͽ�����.", inet_ntoa(pSocketContext->m_ClientAddr.sin_addr),	ntohs(pSocketContext->m_ClientAddr.sin_port));
				// �ͷŵ���Ӧ����Դ
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
					logger->warningLog( "û��send");
				}
				break;
				default:
					logger->warningLog( "_WorkThread�е�	pIoContext->m_OpType �����쳣.");
					break;
				} //switch
			}//if
		}//if

	}//while

	logger->warningLog( "�������߳��˳�.");
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
