#ifndef HAbstractSocket_H__
#define HAbstractSocket_H__

/********************************************************************
	created:	2014/10/11
	created:	11:10:2014   15:41
	file ext:	h
	author:		CHE
	
	purpose:	网络连接抽象类
				子类在继承的时候需要自己写接收回调函数——制定接收策略
				只有绑定了回调函数，本类才会正常工作
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

	/*设置回调函数*/
	void setRecvFunction(Recv_Tcp_Function &&func);
	void setRecvFunction(Recv_Udp_Function &&func);

	/*tcp 发送数据*/
	size_t write(const char* data, size_t dataSize);
	size_t write(const string &buffer);

	/*关闭套接字，并清理资源*/
	void close();

	/*仅关闭套接字*/
	void abort();
	/*获取远程主机address*/
	const HHostAddress* peerAddress()const;

	/*获取本地主机address*/
	const HHostAddress* localAddress()const;

	/*将socket绑定到指定地址和端口*/
	bool bind(const HHostAddress &address, uint16 port = 0);
	bool bind(HHostAddress &&address, uint16 port = 0);
	/*将socket绑定到本机任意地址和指定端口*/
	void bind(uint16 port = 0);

	/*连接到远程主机*/
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
	//由HTcpServer调用此方法
	void readyToReadByServerExecuted();
private:
	unique_ptr<HAbstractSocketPrivate> d_ptr;
};
CHE_NAMESPACE_END
#endif // socket_H__