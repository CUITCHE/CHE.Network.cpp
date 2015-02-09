#include "HTcpServer.h"
#include "HByteConvert.h"
#include "HNetworkDefender.h"

NAMESPACE_BEGIN_CHE


void HTcpServer::HTcpServer_impl(Socket_Pending_Function &&func)
{
	WSADATA wsd;
	if (WSAStartup(0x0202, &wsd) < 0) {
		Warning("socket init failed!");
	}
	Q_D(HTcpServer);
	_memory_dynamic_crt(d->socket_pending_func, move_quick(func));
}

HTcpServer::~HTcpServer()
{
	close();
}

void HTcpServer::close()
{
	Q_D(HTcpServer);
	if (d->listen_fd == INVALID_SOCKET) {
		return;
	}
	int ret = shutdown(d->listen_fd, SD_BOTH);
	//停止监听
	d->listen_shutdown = 1;

	if (ret != 0) {
		Warning("shutdown socket failed..code:%", WSAGetLastError());
		return;
	}
	ret = closesocket(d->listen_fd);
	if (ret != 0) {
		Warning("close socket failed..code:%", WSAGetLastError());
	}
	d->listen_fd = INVALID_SOCKET;
	WSACleanup();
}

bool HTcpServer::listen(const HHostAddress &address, uint16 port /*= 0*/)
{
	Q_D(HTcpServer);
	_memory_dynamic_crt(d->local_address);
	*d->local_address = address;
	d->local_address->port(port);

	return listen_impl();
}

bool HTcpServer::listen(HHostAddress && address, uint16 port)
{
	Q_D(HTcpServer);
	_memory_dynamic_crt(d->local_address);
	*d->local_address = move_quick(address);
	d->local_address->port(port);

	return listen_impl();
}

void HTcpServer::cycle_accept_thread()
{
	Q_D(HTcpServer);
	int addr_len = sizeof(sockaddr);
	SOCKET peer_fd = INVALID_SOCKET;
	HHostAddress peer_address;
	auto &fd = d->listen_fd;
	forever{
		peer_fd = WSAAccept(fd,peer_address.sockAddr(),&addr_len,&HTcpServer::condtionFunc,(DWORD_PTR)this);
		if (peer_fd == INVALID_SOCKET || d->listen_shutdown == 1) {
			break;
		}
		shared_ptr<HTcpSocket> tcp_scoket_tmp(new HTcpSocket());
		tcp_scoket_tmp->d_func()->fd = peer_fd;
		tcp_scoket_tmp->d_func()->peer_address = new HHostAddress(peer_address);
		tcp_scoket_tmp->readyToReadByServerExecuted();
		if (d->socket_pending_func) (*d->socket_pending_func)(tcp_scoket_tmp);
	}
}

int CALLBACK HTcpServer::condtionFunc(LPWSABUF lpCallerId, LPWSABUF lpCallerData, LPQOS lpSQOS, LPQOS lpGQOS, LPWSABUF lpCalleeId, LPWSABUF lpCalleeData, GROUP FAR *g, DWORD_PTR dwCallbackData)
{
	//CF_ACCEPT CF_REJECT
	HTcpServer *pThis = (HTcpServer *)dwCallbackData;

	static HHostAddress addr;
	GetPeerIpv4Address(lpCallerId, addr);

	if (pThis->d_func()->net_defender->Ipv4AddressAuthenticate(addr.toIPv4Address(), addr.port()) == true) {
		return CF_ACCEPT;
	}
	else {
		return CF_REJECT;
	}
}

void HTcpServer::GetPeerIpv4Address(const LPWSABUF buffer, HHostAddress &addr)
{
	addr.clear();
	addr.addr->sin_family = HByteConvert::toType<uint16>((byte *)buffer->buf);
	addr.addr->sin_port = HByteConvert::toType<uint16>((byte *)buffer->buf + sizeof(uint16));
	addr.addr->sin_addr.S_un.S_addr = HByteConvert::toType<uint32>((byte *)buffer->buf + sizeof(uint16) * 2);
}

bool HTcpServer::listen_impl()
{
	Q_D(HTcpServer);
	if (d->listen_fd == INVALID_SOCKET) {
		d->listen_fd = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
		BOOL bDontLinger = FALSE;
		//设置在调用closesocket的时候，会立马关闭
		setsockopt(d->listen_fd, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
	}

	if (::bind(d->listen_fd, d->local_address->sockAddr(), sizeof(sockaddr_in)) < 0) {
		Warning("tcp server address:% port:% bind failed", d->local_address->toString(), d->local_address->port());
		return false;
	}
	if (::listen(d->listen_fd, 5) < 0) {
		Warning("listening failed");
		return false;
	}
	_memory_dynamic_crt(d->net_defender);
	HThread::run(&HTcpServer::cycle_accept_thread, this);
	return true;
}
NAMESPACE_END_CHE