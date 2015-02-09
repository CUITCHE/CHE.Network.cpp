#ifndef	HIOCP_H__
#define	HIOCP_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014	 20:19
	file base:	HIOCP
	author:		CHE

	purpose:	iocpģ�ͣ�Windows NT �汾�ſ�ʹ��
				ʹ����CSDN�û�PiggyXP�Ĵ��룬�ο���http://blog.csdn.net/piggyxp/article/details/6922277
				�Լ��޸�һ����
	*********************************************************************/
#ifndef	WIN32
#error ��win32ƽ̨����ʹ��iocpģ��
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
	HANDLE m_hShutdownEvent = nullptr;		// ����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�
	HANDLE m_hIOCompletionPort = nullptr;	// ��ɶ˿ڵľ��
	HANDLE* m_phWorkerThreads = nullptr;// �������̵߳ľ��ָ��
	int m_nThreads = 0;					// ���ɵ��߳�����
	HHostAddress *m_address = nullptr;	//�������˵�ip�Ͷ˿���Ϣ
	mutex mtx;							// ����Worker�߳�ͬ���Ļ�����

	list<SocketContext *>	*m_arrayClientContext;	// �ͻ���Socket��Context��Ϣ
	SocketContext *m_pListenContext;						// ���ڼ�����Socket��Context��Ϣ
	LPFN_ACCEPTEX m_lpfnAcceptEx;							// AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockAddrs;
};
class HIOCP final
{
public:
	HIOCP(HHostAddress &&address, uint16 port = 0);
	~HIOCP();

	//��������
	bool start();

	//�������ݣ�����֤�����������ͣ����򷵻�SOCKET_ERROR����С��len����
	uint32 write(SOCKET s, const char *buffer, uint32 len);

	//ֹͣ����
	void stop();

	// ��ñ�����IP��ַ
	const char*	getLocalIP();

	//���iocp�Ĺ�����������
	unique_ptr<HighEffectIOBuffer>& buffer_block() { return shared_buffer_block; }
protected:
	// ����Socket��
	bool loadSocketLib();

	// ж��Socket�⣬��������
	void unloadSocketLib() { WSACleanup(); }

	// ��ʼ��IOCP
	bool initializeIOCP();

	// ��ʼ��Socket
	bool initializeListenSocket();

	// ����ͷ���Դ
	void delInitialize();

	// Ͷ��Accept����
	bool postAccept(IOContext *pAcceptIoContext);

	// Ͷ�ݽ�����������
	bool postRecv(IOContext *pIoContext);

	// ���пͻ��������ʱ�򣬽��д���
	bool doAccpet(SocketContext *pSocketContext, IOContext *pIoContext);

	// ���н��յ����ݵ����ʱ�򣬽��д���
	bool doRecv(SocketContext *pSocketContext, IOContext *pIoContext);

	// ���ͻ��˵������Ϣ�洢��������
	void addToContextList(SocketContext *pSocketContext);

	// ���ͻ��˵���Ϣ���������Ƴ�
	void removeContext(SocketContext *pSocketContext);

	// ��տͻ�����Ϣ
	void clearContextList();

	// ������󶨵���ɶ˿���
	bool associateWithIOCP(SocketContext *pContext);

	// ������ɶ˿��ϵĴ���
	bool handleError(SocketContext *pContext, const DWORD& dwErr);

	// �̺߳�����ΪIOCP�������Ĺ������߳�
	static DWORD WINAPI workerThread(LPVOID lpParam);

	// ��ñ����Ĵ���������
	int	getNoOfProcessors();

	// �жϿͻ���Socket�Ƿ��Ѿ��Ͽ�
	bool isSocketAlive(SOCKET s);
private:
	unique_ptr<HIOCPdata> d_ptr;
	unique_ptr<HighEffectIOBuffer> shared_buffer_block;
};
NAMESPACE_END_CHE
#endif // HIOCP_H__
