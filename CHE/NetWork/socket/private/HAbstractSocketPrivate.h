#include "HThread.h"
#include "HHostAddress.h"

NAMESPACE_BEGIN_CHE

class HAbstractSocket;
class HHostAddress;

typedef std::function<void(const char *buffer, size_t len)> Recv_Tcp_Function;		//tcp接收数据函数对象
typedef std::function<void(const char *buffer, size_t len, HHostAddress &address)> Recv_Udp_Function;		//udp接收数据函数对象

CHE_PRIVATE_DEFINED(HAbstractSocket)
{
	const SOCK_TYPE socket_type;				//sock类型 只能是tcp or udp
	SOCKET fd;									//socket 文件描述符
	HHostAddress *peer_address = nullptr;		//远程主机地址对象
	HHostAddress *local_address = nullptr;		//本地主机地址对象
	Recv_Tcp_Function *recv_func_tcp = nullptr;	//
	Recv_Udp_Function *recv_func_udp = nullptr;	//
	HAbstractSocketPrivate(const SOCK_TYPE type);
	operator SOCKET()const;
};

NAMESPACE_END_CHE