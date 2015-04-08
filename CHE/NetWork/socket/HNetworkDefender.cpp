#include "HNetworkDefender.h"

CHE_NAMESPACE_BEGIN


HNetworkDefender::HNetworkDefender(HObject *parent /*= 0*/)
	:ipv4Addre_accounts(new Ipv4Address_Acount_Map())
{
}

HNetworkDefender::HNetworkDefender(HNetworkDefender &&rhs)
	: ipv4Addre_accounts(move_quick(rhs.ipv4Addre_accounts))
{
}

void HNetworkDefender::operator=(HNetworkDefender &&rhs)
{
	this->ipv4Addre_accounts = move_quick(rhs.ipv4Addre_accounts);
}

HNetworkDefender::~HNetworkDefender()
{

}

bool HNetworkDefender::Ipv4AddressAuthenticate(Ipv4Address_Type ipAddress, Ipv4Port_Type port)
{
	auto iter = ipv4Addre_accounts->find(ipAddress);

	if (iter == ipv4Addre_accounts->end()) {
		Ipv4Address_Value_Type tmp(new HNetworkAddressManager(ipAddress, port));
		ipv4Addre_accounts->insert(pair<Ipv4Address_Type, Ipv4Address_Value_Type>(ipAddress, move_quick(tmp)));
		return true;
	}
	auto &right = iter->second;
	right->connected(port);
	return *right;
}

bool HNetworkDefender::Ipv4AddressQuit(Ipv4Address_Type ipAddress, Ipv4Port_Type port)
{
	auto iter = ipv4Addre_accounts->find(ipAddress);
	if (iter == ipv4Addre_accounts->end())	return false;
	iter->second->disconnected(port);
	return true;
}

CHE_NAMESPACE_END
