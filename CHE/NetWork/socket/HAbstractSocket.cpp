#include "HAbstractSocket.h"
#include <string>
CHE_NAMESPACE_BEGIN
/*
#pragma region ERROR_STRING
const std::map<int, const char *> inet_error_string = {
		{ WSAEFAULT, "��ַ��Ч" },
		{ WSA_INVALID_HANDLE, "ָ�����¼�������Ч" },
		{ WSA_NOT_ENOUGH_MEMORY, "�ڴ治��" },
		{ WSA_INVALID_PARAMETER, "һ������������Ч" },
		{ WSA_WAIT_TIMEOUT, "������ʱ" },
		{ WSA_OPERATION_ABORTED, "�ص�������ȡ��" },
		{ WSA_IO_INCOMPLETE, "�ص�I/O�¼�����δ���ڴ���״̬" },
		{ WSA_IO_PENDING, "�ص����������Ժ����" },
		{ WSAEINTR, "���������ж�" },
		{ WSAEBADF, "�ļ��������" },
		{ WSAEACCES, "Ȩ�ޱ���" },
		{ WSAEINVAL, "������Ч" },
		{ WSAEMFILE, "���ļ�����" },
		{ WSAEWOULDBLOCK, "��Դ��ʱ������" },
		{ WSAEINPROGRESS, "�������ڽ�����" },
		{ WSAEALREADY, "��������ɣ������ظ�����" },
		{ WSAENOTSOCK, "��Ч��socket" },
		{ WSAEDESTADDRREQ, "��Ч��Ŀ���ַ" },
		{ WSAEMSGSIZE, "��Ϣ����" },
		{ WSAEPROTOTYPE, "�׽���Э����������" },
		{ WSAENOPROTOOPT, "��getsockopt��setsockopt������Э��ѡ�����" },
		{ WSAEPROTONOSUPPORT, "��֧�ֵ�Э��" },
		{ WSAESOCKTNOSUPPORT, "��֧�ֵ��׽�������" },
		{ WSAEOPNOTSUPP, "��֧�ֵĲ���" },
		{ WSAEAFNOSUPPORT, "��ַ���岻֧������Ĳ���" },
		{ WSAEADDRINUSE, "socket��ַ����ʹ��.����setsockopt�����������׽���ѡ��SO_REUSEADDR���������׽��ַ���ͬһ������IP��ַ���˿ں�" },
		{ WSAEADDRNOTAVAIL, "���ܷ�������ĵ�ַ" },
		{ WSAENETDOWN, "����Ͽ�" },
		{ WSAENETUNREACH, "���粻�ɵִ�" },
		{ WSAENETRESET, "��������ʱ�Ͽ�������" },
		{ WSAECONNABORTED, "����������ȡ��" },
		{ WSAECONNRESET, "���ӱ��Է�����" },
		{ WSAENOBUFS, "û���㹻�Ļ������ռ�" },
		{ WSAEISCONN, "�׽����Ѿ�����" },
		{ WSAENOTCONN, "TCP Socket�׽�����δ����" },
		{ WSAESHUTDOWN, "�׽��ֹرպ��ܷ���" },
		{ WSAETIMEDOUT, "���ӳ�ʱ" },
		{ WSAECONNREFUSED, "���ӱ���" },
		{ WSAEHOSTDOWN, "�����ر�" },
		{ WSAEHOSTUNREACH, "û�е�������·��" },
		{ WSAEPROCLIM, "���̹���" },
		{ WSASYSNOTREADY, "������ϵͳ������" },
		{ WSAVERNOTSUPPORTED, "Winsock.dll�汾����" },
		{ WSANOTINITIALISED, "Winsock��δ��ʼ��" },
		{ WSAEDISCON, "���ڴ��ݹر�" },
		{ WSAENOMORE, "�Ҳ�������ļ�¼" },
		{ WSAECANCELLED, "������ȡ��" },
		{ WSAEINVALIDPROVIDER, "��Ч�ķ����ṩ��" },
		{ WSAEPROVIDERFAILEDINIT, "�ṩ�߳�ʼ��ʧ��" },
		{ WSASYSCALLFAILURE, "ϵͳ����ʧ��" },
		{ WSASERVICE_NOT_FOUND, "�Ҳ��������ķ���" },
		{ WSATYPE_NOT_FOUND, "�Ҳ����������" },
		{ WSA_E_NO_MORE, "�Ҳ�������ļ�¼" },
		{ WSA_E_CANCELLED, "������ȡ��" },
		{ WSAEREFUSED, "��ѯ����" },
		{ WSAHOST_NOT_FOUND, "����û���ҵ�" },
		{ WSATRY_AGAIN, "����Ȩ����û���ҵ�" },
		{ WSANO_RECOVERY, "����һ�����ɻָ��Ĵ���" },
		{ WSANO_DATA, "û���ҵ��������͵����ݼ�¼" },
		{ WSA_QOS_RECEIVERS, "������һ��ԤԼ��Ϣ�ִ�" },
		{ WSA_QOS_SENDERS, "WSA_QOS_SENDERS" },
		{ WSA_QOS_NO_SENDERS, "û��QoS������" },
		{ WSA_QOS_NO_RECEIVERS, "û��QoS������" },
		{ WSA_QOS_REQUEST_CONFIRMED, "ԤԼ�����ѱ�ȷ��" },
		{ WSA_QOS_ADMISSION_FAILURE, "ȱ����Դ�´�" },
		{ WSA_QOS_POLICY_FAILURE, "֤����Ч" },
		{ WSA_QOS_BAD_STYLE, "δ֪���ͻ����ʽ" },
		{ WSA_QOS_BAD_OBJECT, "��Ч��FILTERSPEC�ṹ�����ṩ�����ж���" },
		{ WSA_QOS_TRAFFIC_CTRL_ERROR, " FLOWSPEC������" },
		{ WSA_QOS_GENERIC_ERROR, "����QoS����" }
};
#pragma endregion ERROR_STRING
*/
#pragma region GLOBAL_VAR_INIT

HAbstractSocket::HAbstractSocket(SOCK_TYPE type, HObject *parent /*= 0*/)
	:HThread(parent)
	,d_ptr(new HAbstractSocketPrivate(type))
{
	Q_D(HAbstractSocket);
	d->fd = getOneSocket();
}

SOCKET HAbstractSocket::getOneSocket()
{
	Q_D(HAbstractSocket);
	WSADATA wsd;
	if (WSAStartup(0x0202, &wsd) < 0){
		Warning("socket init failed!");
		return INVALID_SOCKET;
	}
	SOCKET fdt = INVALID_SOCKET;
	if (d->socket_type == TcpSocket){
		fdt = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	}
	else {
		fdt = WSASocketW(AF_INET, SOCK_DGRAM, 0, NULL, 0, 0);
		HThread::run(&HAbstractSocket::cycle_read_udp, this);
	}
	setOneSocket(fdt);
	return fdt;
}

void HAbstractSocket::setOneSocket(SOCKET fdt)
{
	int nNetTimeout = 0;//���û�����ݵ�������������
	setsockopt(fdt, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	BOOL bDontLinger = FALSE;
	//�����ڵ���closesocket��ʱ�򣬻�����ر�
	setsockopt(fdt, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
}

HAbstractSocket::~HAbstractSocket()
{
	close();
}

bool HAbstractSocket::bind(const HHostAddress &address, uint16 port /*= 0*/)
{
	Q_D(HAbstractSocket);

	if (d->socket_type == TcpSocket){
		return false;
	}
	_memory_dynamic_crt(d->local_address);
	*d->local_address = address;
	d->local_address->port(port);
	return bind_impl();
}

bool HAbstractSocket::bind(HHostAddress && address, uint16 port)
{
	Q_D(HAbstractSocket);

	if (d->socket_type == TcpSocket) {
		return false;
	}
	_memory_dynamic_crt(d->local_address);
	*d->local_address = move_quick(address);
	d->local_address->port(port);
	return bind_impl();
}

bool HAbstractSocket::bind_impl()
{
	Q_D(HAbstractSocket);

	int ret = ::bind(d->fd, d->local_address->sockAddr(), sizeof(sockaddr_in));
	if (ret != 0) {
		Warning("bind % to % port failed..code:%", d->local_address->toString(), d->local_address->port(), WSAGetLastError());
		return false;
	}
	return true;
}

void HAbstractSocket::bind(uint16 port)
{
	bind(HHostAddress((uint32)INADDR_ANY), port);
}

bool HAbstractSocket::connectToHost(const char * hostName, uint16 port)
{
	return connectToHost(HHostAddress(hostName), port);
}

bool HAbstractSocket::connectToHost(const HHostAddress &address, uint16 port)
{
	Q_D(HAbstractSocket);

	if (d->socket_type == UdpSocket) {
		return false;
	}
	_memory_dynamic_crt(d->peer_address);
	*d->peer_address = address;
	d->peer_address->port(port);
	return connectToHost_impl();
}

bool HAbstractSocket::connectToHost(HHostAddress &&address, uint16 port)
{
	Q_D(HAbstractSocket);
	if (d->socket_type == UdpSocket) {
		return false;
	}
	_memory_dynamic_crt(d->peer_address);
	*d->peer_address = move_quick(address);
	d->peer_address->port(port);
	return connectToHost_impl();
}

void HAbstractSocket::close()
{
	this->abort();
	WSACleanup();
}

void HAbstractSocket::abort()
{
	Q_D(HAbstractSocket);
	auto &fd = d->fd;
	if (fd == INVALID_SOCKET)	return;
	int ret = shutdown(fd, SD_BOTH);
	//�����������룬ȥstatic��¼��ɾ����
	if (ret != 0){
		Warning("shutdown socket failed..code:%", WSAGetLastError());
		return;
	}
	ret = closesocket(fd);
	if (ret != 0){
		Warning("close socket failed..code:%", WSAGetLastError());
	}
	fd = INVALID_SOCKET;
}

const HHostAddress* HAbstractSocket::peerAddress() const
{
	Q_DC(HAbstractSocket);
	return d->peer_address;
}

const HHostAddress* HAbstractSocket::localAddress() const
{
	Q_DC(HAbstractSocket);
	return d->local_address;
}

void HAbstractSocket::setRecvFunction(Recv_Tcp_Function &&func)
{
	Q_D(HAbstractSocket);
	_memory_dynamic_crt(d->recv_func_tcp);
	*d->recv_func_tcp = move_quick(func);
}

void HAbstractSocket::setRecvFunction(Recv_Udp_Function &&func)
{
	Q_D(HAbstractSocket);
	_memory_dynamic_crt(d->recv_func_udp);
	*d->recv_func_udp = move_quick(func);
}

size_t HAbstractSocket::write(const char* data, size_t dataSize)
{
	Q_D(HAbstractSocket);
	size_t senCnt = 0;
	int tmpSenCnt = 0;

	while (senCnt < dataSize){
		tmpSenCnt = send(d->fd, data + senCnt, dataSize - senCnt, 0);
		if (tmpSenCnt == SOCKET_ERROR) {
			break;
		}
		senCnt += tmpSenCnt;
	}
	return senCnt;
}

size_t HAbstractSocket::write(const string & buffer)
{
	return write(buffer.data(), buffer.size());
}

size_t HAbstractSocket::writeDatagram(const char *data, size_t dataSize, const HHostAddress &address)
{
	Q_D(HAbstractSocket);
	size_t senCnt = 0;
	int tmpSenCnt = 0;
	while (senCnt < dataSize){
		tmpSenCnt = sendto(d->fd, data + senCnt, dataSize - senCnt, 0, address.sockAddr(), sizeof(sockaddr_in));
		if (tmpSenCnt == SOCKET_ERROR) {
			break;
		}
		senCnt += tmpSenCnt;
	}
	return senCnt;
}
bool HAbstractSocket::connectToHost_impl()
{
	Q_D(HAbstractSocket);
	//������������
	{
		BOOL bConditionalAccept = TRUE;
		setsockopt(d->fd, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (const char*)&bConditionalAccept, sizeof(BOOL));
	}
	int ret = WSAConnect(d->fd, d->peer_address->sockAddr(), sizeof(sockaddr), NULL, NULL, 0, 0);
	if (ret != 0) {
		Warning("connected faild!");
		return false;
	}
	HThread::run(&HAbstractSocket::cycle_read_tcp, this);
	return true;
}

void HAbstractSocket::cycle_read_tcp()
{
	Q_D(HAbstractSocket);
	int ret = 0;
	const int length = 32 * 1024;
	char buffer[length];
	auto &recv_func_tcp = d->recv_func_tcp;
	auto fd = d->fd;
	forever{
		ret = recv(fd,buffer,length,0);
		if (ret == SOCKET_ERROR) {
			break;
		}
		//abort���Ƶı���
		//TODO:

		ret > 0 && recv_func_tcp ? (*recv_func_tcp)(buffer, ret) : che_noop();
	}
}

void HAbstractSocket::cycle_read_udp()
{
	Q_D(HAbstractSocket);
	int ret = 0;
	const int length = 32 * 1024;
	char buffer[length];
	int addr_len = sizeof(sockaddr_in);
	auto &recv_func_udp = d->recv_func_udp;
	auto &peer_address = d->peer_address;
	auto fd = d->fd;
	forever{
		ret = recvfrom(fd, buffer, length, 0, peer_address->sockAddr(),&addr_len);
		if (ret == SOCKET_ERROR) {
			break;
		}
		ret > 0 && recv_func_udp ? (*recv_func_udp)(buffer, ret, *peer_address) : che_noop();
	}
}

bool HAbstractSocket::isValid() const
{
	Q_DC(HAbstractSocket);
	return INVALID_SOCKET != d->fd;
}

void HAbstractSocket::readyToReadByServerExecuted()
{
	HThread::run(&HAbstractSocket::cycle_read_tcp, this);
}

CHE_NAMESPACE_END
