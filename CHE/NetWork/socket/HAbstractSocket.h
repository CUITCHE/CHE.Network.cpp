#ifndef HAbstractSocket_H__
#define HAbstractSocket_H__

/********************************************************************
	created:	2014/10/11
	created:	11:10:2014   15:41
	file ext:	h
	author:		CHE
	
	purpose:	�������ӳ�����
				�����ڼ̳е�ʱ����Ҫ�Լ�д���ջص����������ƶ����ղ���
				ֻ�а��˻ص�����������Ż���������
*********************************************************************/
#include "../NetWork/socket/private/HAbstractSocketPrivate.h"
#include <map>
using std::map;
#pragma comment(lib, "ws2_32.lib")

CHE_NAMESPACE_BEGIN

class HAbstractSocket : public HThread
{
	CHE_PRIVATE_DECLARE(HAbstractSocket)
	CHE_DISABLE_COPY(HAbstractSocket)
	CHE_DISABLE_COPY_RREF(HAbstractSocket)
	friend class HTcpServer;
public:
	~HAbstractSocket() override;

	/*Returns true if the socket is valid and ready for use; otherwise returns false.*/
	bool isValid()const;

	/*���ûص�����*/
	void setRecvFunction(Recv_Tcp_Function &&func);
	void setRecvFunction(Recv_Udp_Function &&func);

	/*tcp ��������*/
	size_t write(const char* data, size_t dataSize);
	size_t write(const string &buffer);

	/*�ر��׽��֣���������Դ*/
	void close();

	/*���ر��׽���*/
	void abort();
	/*��ȡԶ������address*/
	const HHostAddress* peerAddress()const;

	/*��ȡ��������address*/
	const HHostAddress* localAddress()const;

	/*��socket�󶨵�ָ����ַ�Ͷ˿�*/
	bool bind(const HHostAddress &address, uint16 port = 0);
	bool bind(HHostAddress &&address, uint16 port = 0);
	/*��socket�󶨵����������ַ��ָ���˿�*/
	void bind(uint16 port = 0);

	/*���ӵ�Զ������*/
	bool connectToHost(const char *hostName, uint16 port);
	bool connectToHost(const HHostAddress &address, uint16 port);
	bool connectToHost(HHostAddress &&address, uint16 port);

protected:
	HAbstractSocket(SOCK_TYPE type, HObject *parent = 0);
	SOCKET getOneSocket();
	void setOneSocket(SOCKET fdt);
	void cycle_read_tcp();
	void cycle_read_udp();
	size_t writeDatagram(const char *data, size_t dataSize, const HHostAddress &address);
	size_t writeDatagram(const string &buffer, const HHostAddress &address){ return writeDatagram(buffer.data(), buffer.size(), address); }
	bool connectToHost_impl();
	bool bind_impl();
	//��HTcpServer���ô˷���
	void readyToReadByServerExecuted();
private:
	unique_ptr<HAbstractSocketPrivate> d_ptr;
};
CHE_NAMESPACE_END
#endif // socket_H__