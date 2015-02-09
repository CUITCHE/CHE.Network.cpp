#include "HThread.h"
#include "HHostAddress.h"

NAMESPACE_BEGIN_CHE

class HAbstractSocket;
class HHostAddress;

typedef std::function<void(const char *buffer, size_t len)> Recv_Tcp_Function;		//tcp�������ݺ�������
typedef std::function<void(const char *buffer, size_t len, HHostAddress &address)> Recv_Udp_Function;		//udp�������ݺ�������

CHE_PRIVATE_DEFINED(HAbstractSocket)
{
	const SOCK_TYPE socket_type;				//sock���� ֻ����tcp or udp
	SOCKET fd;									//socket �ļ�������
	HHostAddress *peer_address = nullptr;		//Զ��������ַ����
	HHostAddress *local_address = nullptr;		//����������ַ����
	Recv_Tcp_Function *recv_func_tcp = nullptr;	//
	Recv_Udp_Function *recv_func_udp = nullptr;	//
	HAbstractSocketPrivate(const SOCK_TYPE type);
	operator SOCKET()const;
};

NAMESPACE_END_CHE