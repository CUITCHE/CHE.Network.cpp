#ifndef HHostAddress_H__
#define HHostAddress_H__
//
/********************************************************************
	created:	2014/10/11
	created:	11:10:2014   21:32
	file ext:	hpp
	author:		CHE
	
	purpose:	host地址结构类,含ip地址和端口
*********************************************************************/
#include "hglobal.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <list>
using std::list;

NAMESPACE_BEGIN_CHE

enum SOCK_TYPE{
	UdpSocket,
	TcpSocket
};

class HAbstractSocket;
class HTcpServer;

class HHostAddress
{
	friend class HAbstractSocket;
	friend class HTcpServer;
public:
	HHostAddress();
	HHostAddress(uint32 ip4Addr, uint16 port = 0);
	HHostAddress(const char *address, uint16 port = 0);
#pragma region r_copy_func
	HHostAddress(HHostAddress &&rhs);
	HHostAddress& operator=(HHostAddress &&rhs);
#pragma  endregion
#pragma region copy_func
	HHostAddress(const HHostAddress &rhs);
	HHostAddress& operator=(const HHostAddress &rhs);
#pragma endregion

	~HHostAddress(){ 
		che_array_pointer_del(inet_ip_str);
		che_pointer_del(addr);
	}
	static list<uint32> getHostAddressByHostName(const char* pName, SOCK_TYPE socket_type = TcpSocket);
public:
	//结构体为空，返回true
	H_INLINE bool isNull()const{ return !null; }
	H_INLINE void clear(){ CheZeroMemory(addr, sizeof(sockaddr_in)); null = false;}

	void setAddress(uint32 ip4Addr, uint16 port = 0);
	void setAddress(const char *address, uint16 port = 0);
	void setAddress(const sockaddr_in *addr);
	uint32 toIPv4Address()const;
	const char *toString();

	//获得sockaddr指针
	H_INLINE const sockaddr* sockAddr()const{ return (const sockaddr*)addr; }
	H_INLINE sockaddr*sockAddr(){ return (sockaddr*)addr; }

	//设置端口
	void port(uint16 port);
	uint16 port()const;
private:
	HHostAddress(sockaddr_in *_addr);
private:
	uint32 null : 1;			//
	uint32 needRecalc : 1;		//标记是否需要重新计算inet_ip_str
	uint32 _zero : 30;
	char *inet_ip_str = nullptr;//4
	sockaddr_in *addr = nullptr;//16,*4
};
NAMESPACE_END_CHE
#endif // HHostAddress_H__
