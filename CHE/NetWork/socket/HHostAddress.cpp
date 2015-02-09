#include "HHostAddress.h"
#include "HLog.h"
#include <algorithm>
NAMESPACE_BEGIN_CHE

HHostAddress::HHostAddress()
	:null(0)
	, needRecalc(1)
	, addr(new sockaddr_in)
{
	CheZeroMemory(addr, sizeof(sockaddr_in));
}

HHostAddress::HHostAddress(uint32 ip4Addr, uint16 port /*= 0*/)
	:null(0)
	, needRecalc(1)
	, addr(new sockaddr_in)
{
	this->setAddress(ip4Addr, port);
}

HHostAddress::HHostAddress(const char *address, uint16 port /*= 0*/)
	:null(0)
	, needRecalc(1)
	, addr(new sockaddr_in)
{
	this->setAddress(address, port);
}

HHostAddress::HHostAddress(HHostAddress &&rhs)
	: null(rhs.null)
	, addr(rhs.addr)
{
	rhs.addr = nullptr;
	if (rhs.needRecalc == 0){
		std::swap(this->inet_ip_str, rhs.inet_ip_str);
	}
}

HHostAddress::HHostAddress(const HHostAddress &rhs)
	: null(rhs.null)
	, needRecalc(1)
	, addr(new sockaddr_in)
{
	*addr = *rhs.addr;
}

HHostAddress::HHostAddress(sockaddr_in *_addr)
	: null(0)
	, needRecalc(1)
	, addr(_addr)
{

}

HHostAddress& HHostAddress::operator=(HHostAddress &&rhs)
{
	null = rhs.null;
	//此移动赋值函数下的addr=nullptr
	std::swap(this->addr, rhs.addr);
	if (rhs.needRecalc == 0){
		std::swap(this->inet_ip_str, rhs.inet_ip_str);
	}
	return *this;
}

HHostAddress& HHostAddress::operator=(const HHostAddress &rhs)
{
	null = rhs.null;
	needRecalc = 1;
	*addr = *rhs.addr;
	return *this;
}

void HHostAddress::setAddress(uint32 ip4Addr, uint16 port /*= 0*/)
{
	CheZeroMemory(addr, sizeof(sockaddr_in));

	addr->sin_family = AF_INET;
	addr->sin_addr.S_un.S_addr = htonl(ip4Addr);
	addr->sin_port = htons(port);
	null = true;
	needRecalc = 1;
}

void HHostAddress::setAddress(const char *address, uint16 port /*= 0*/)
{
	CheZeroMemory(addr, sizeof(sockaddr_in));

	addr->sin_family = AF_INET;
	inet_pton(AF_INET, address, (void *)&addr->sin_addr) > 0 ? null = true : null = false;
	addr->sin_port = htons(port);
	needRecalc = 1;
}

void HHostAddress::setAddress(const sockaddr_in * _addr)
{
	memcpy(this->addr, _addr, sizeof(sockaddr_in));
	needRecalc = 1;
}

const char * HHostAddress::toString()
{
	if (inet_ip_str == nullptr) {
		inet_ip_str = new char[16];
	}

	if (needRecalc == 1){
		inet_ntop(AF_INET, (void*)&addr->sin_addr, inet_ip_str, 16);
		needRecalc = 0;
	}
	return inet_ip_str;
}

uint32 HHostAddress::toIPv4Address() const
{
	return ntohl(addr->sin_addr.S_un.S_addr);
}

void HHostAddress::port(uint16 port)
{
	addr->sin_port = htons(port);
}

uint16 HHostAddress::port() const
{
	return ntohs(addr->sin_port);
}

list<uint32> HHostAddress::getHostAddressByHostName(const char* pName, SOCK_TYPE socket_type /*= TcpSocket*/)
{
	addrinfo hints;
	WSADATA wsaData;

	CheZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_socktype = socket_type == TcpSocket ? SOCK_STREAM : SOCK_DGRAM;

	list<uint32> relist;

	addrinfo *res = nullptr;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		logger->warningLog( "WSAStartup failed:", iResult);
		return relist;
	}
	int ret = getaddrinfo(pName, NULL, &hints, &res);
	if (ret != 0){
		logger->warningLog("host name:", pName," may be not a correct name.code:", ret);
		return (relist);
	}
	auto p = res;
	while (p){
		relist.push_back(ntohl(((sockaddr_in *)(p->ai_addr))->sin_addr.S_un.S_addr));
		p = p->ai_next;
	}
	freeaddrinfo(res);
	WSACleanup();
	return (relist);
}

NAMESPACE_END_CHE