#include "HAbstractSocket.h"
#include <string>
CHE_NAMESPACE_BEGIN
/*
#pragma region ERROR_STRING
const std::map<int, const char *> inet_error_string = {
		{ WSAEFAULT, "地址无效" },
		{ WSA_INVALID_HANDLE, "指定的事件对象无效" },
		{ WSA_NOT_ENOUGH_MEMORY, "内存不够" },
		{ WSA_INVALID_PARAMETER, "一个或多个参数无效" },
		{ WSA_WAIT_TIMEOUT, "操作超时" },
		{ WSA_OPERATION_ABORTED, "重叠操作被取消" },
		{ WSA_IO_INCOMPLETE, "重叠I/O事件对象未处于传信状态" },
		{ WSA_IO_PENDING, "重叠操作将在以后完成" },
		{ WSAEINTR, "函数调用中断" },
		{ WSAEBADF, "文件句柄错误" },
		{ WSAEACCES, "权限被拒" },
		{ WSAEINVAL, "参数无效" },
		{ WSAEMFILE, "打开文件过多" },
		{ WSAEWOULDBLOCK, "资源暂时不可用" },
		{ WSAEINPROGRESS, "操作正在进行中" },
		{ WSAEALREADY, "操作已完成，请勿重复调用" },
		{ WSAENOTSOCK, "无效的socket" },
		{ WSAEDESTADDRREQ, "无效的目标地址" },
		{ WSAEMSGSIZE, "消息过长" },
		{ WSAEPROTOTYPE, "套接字协议类型有误" },
		{ WSAENOPROTOOPT, "在getsockopt或setsockopt调用中协议选项错误" },
		{ WSAEPROTONOSUPPORT, "不支持的协议" },
		{ WSAESOCKTNOSUPPORT, "不支持的套接字类型" },
		{ WSAEOPNOTSUPP, "不支持的操作" },
		{ WSAEAFNOSUPPORT, "地址家族不支持请求的操作" },
		{ WSAEADDRINUSE, "socket地址正在使用.可在setsockopt函数中设置套接字选项SO_REUSEADDR，允许多个套接字访问同一个本地IP地址及端口号" },
		{ WSAEADDRNOTAVAIL, "不能分配请求的地址" },
		{ WSAENETDOWN, "网络断开" },
		{ WSAENETUNREACH, "网络不可抵达" },
		{ WSAENETRESET, "网络重设时断开了连接" },
		{ WSAECONNABORTED, "软件造成连接取消" },
		{ WSAECONNRESET, "连接被对方重设" },
		{ WSAENOBUFS, "没有足够的缓冲区空间" },
		{ WSAEISCONN, "套接字已经连接" },
		{ WSAENOTCONN, "TCP Socket套接字尚未连接" },
		{ WSAESHUTDOWN, "套接字关闭后不能发送" },
		{ WSAETIMEDOUT, "连接超时" },
		{ WSAECONNREFUSED, "连接被拒" },
		{ WSAEHOSTDOWN, "主机关闭" },
		{ WSAEHOSTUNREACH, "没有到主机的路由" },
		{ WSAEPROCLIM, "进程过多" },
		{ WSASYSNOTREADY, "网络子系统不可用" },
		{ WSAVERNOTSUPPORTED, "Winsock.dll版本有误" },
		{ WSANOTINITIALISED, "Winsock尚未初始化" },
		{ WSAEDISCON, "正在从容关闭" },
		{ WSAENOMORE, "找不到更多的记录" },
		{ WSAECANCELLED, "操作被取消" },
		{ WSAEINVALIDPROVIDER, "无效的服务提供者" },
		{ WSAEPROVIDERFAILEDINIT, "提供者初始化失败" },
		{ WSASYSCALLFAILURE, "系统调用失败" },
		{ WSASERVICE_NOT_FOUND, "找不到这样的服务" },
		{ WSATYPE_NOT_FOUND, "找不到类的类型" },
		{ WSA_E_NO_MORE, "找不到更多的记录" },
		{ WSA_E_CANCELLED, "操作被取消" },
		{ WSAEREFUSED, "查询被拒" },
		{ WSAHOST_NOT_FOUND, "主机没有找到" },
		{ WSATRY_AGAIN, "非授权主机没有找到" },
		{ WSANO_RECOVERY, "遇到一个不可恢复的错误" },
		{ WSANO_DATA, "没有找到请求类型的数据记录" },
		{ WSA_QOS_RECEIVERS, "至少有一条预约消息抵达" },
		{ WSA_QOS_SENDERS, "WSA_QOS_SENDERS" },
		{ WSA_QOS_NO_SENDERS, "没有QoS发送者" },
		{ WSA_QOS_NO_RECEIVERS, "没有QoS接收者" },
		{ WSA_QOS_REQUEST_CONFIRMED, "预约请求已被确认" },
		{ WSA_QOS_ADMISSION_FAILURE, "缺乏资源致错" },
		{ WSA_QOS_POLICY_FAILURE, "证书无效" },
		{ WSA_QOS_BAD_STYLE, "未知或冲突的样式" },
		{ WSA_QOS_BAD_OBJECT, "无效的FILTERSPEC结构或者提供者特有对象" },
		{ WSA_QOS_TRAFFIC_CTRL_ERROR, " FLOWSPEC有问题" },
		{ WSA_QOS_GENERIC_ERROR, "常规QoS错误" }
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
	int nNetTimeout = 0;//如果没有数据到来，立即返回
	setsockopt(fdt, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	BOOL bDontLinger = FALSE;
	//设置在调用closesocket的时候，会立马关闭
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
	//调用其它代码，去static记录中删除它
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
	//设置阻塞连接
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
		//abort控制的变量
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
