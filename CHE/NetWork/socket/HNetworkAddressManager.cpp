#include "HNetworkAddressManager.h"

NAMESPACE_BEGIN_CHE

HNetworkAddressManager::HNetworkAddressManager(HObject *parent /*= 0*/)
:HObject(parent)
, d_ptr(new HNetworkAddressManagerPrivate())
{
}

HNetworkAddressManager::HNetworkAddressManager(Ipv4Address_Type address, Ipv4Port_Type port, HObject *parent /*= 0*/)
	: HObject(parent)
	, d_ptr(new HNetworkAddressManagerPrivate(address, port))
{
}

HNetworkAddressManager::HNetworkAddressManager(HNetworkAddressManager &&rhs)
:d_ptr(move_quick(rhs.d_ptr))
{
}

void HNetworkAddressManager::operator=(HNetworkAddressManager &&rhs)
{
	this->d_ptr = move_quick(rhs.d_ptr);
}

HNetworkAddressManager::~HNetworkAddressManager()
{
	sizeof(list<Ipv4Port_Type>);
}

bool HNetworkAddressManager::empty() const
{
	Q_DC(HNetworkAddressManager);
	return d->ipv4Ports.size() == 0;
}

Ipv4Address_Type HNetworkAddressManager::toIpv4Address() const
{
	Q_DC(HNetworkAddressManager);
	return d->ipv4Addr;
}

bool HNetworkAddressManager::contain(Ipv4Port_Type port)const
{
	Q_DC(HNetworkAddressManager);
	return d->ipv4Ports.end() != std::find(d->ipv4Ports.begin(), d->ipv4Ports.end(), port);
}

void HNetworkAddressManager::disconnected(Ipv4Port_Type port)
{
	Q_D(HNetworkAddressManager);
	auto iter = std::find(d->ipv4Ports.begin(), d->ipv4Ports.end(), port);
	if (iter == d->ipv4Ports.end())	return;
	d->ipv4Ports.erase(iter);
}

bool HNetworkAddressManager::connected(Ipv4Port_Type port)
{
	Q_D(HNetworkAddressManager);
	if (d->authorization == false)	return false;

	time_t cur_t = HTime::currentTime();
	if (cur_t - d->recent_connected_time < d->s_time_limit) {
		return permit(false);
	}
	d->recent_connected_time = cur_t;
	if (contain(port) == true)	return true;
	d->ipv4Ports.push_back(port);
	return true;
}

HNetworkAddressManager::operator bool() const
{
	Q_DC(HNetworkAddressManager);
	return d->authorization;
}

bool HNetworkAddressManager::permit(Authentication mission)
{
	Q_D(HNetworkAddressManager);
	d->authorization = mission;
	d->recent_connected_time = 0;
	if (!mission && d->isDefriendFunctionCallbackValid()) {
		(*d->defriend_func)(d->ipv4Addr);
	}
	sizeof(Defriend_Function);
	return d->authorization;
}

bool HNetworkAddressManager::setLimitTime(const int limit_t)
{
	HNetworkAddressManagerPrivate::s_time_limit = limit_t;
	return true;
}

void HNetworkAddressManager::setDefriendFuncCallback(Defriend_Function &&func)
{
	Q_D(HNetworkAddressManager);
	_memory_dynamic_crt(d->defriend_func, move_quick(func));
}

NAMESPACE_END_CHE