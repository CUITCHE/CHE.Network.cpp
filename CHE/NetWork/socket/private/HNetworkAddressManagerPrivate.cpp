#include "../NetWork/socket/private/HNetworkAddressManagerPrivate.h"

NAMESPACE_BEGIN_CHE
int HNetworkAddressManagerPrivate::s_time_limit = 0;


bool HNetworkAddressManagerPrivate::isDefriendFunctionCallbackValid() const
{
	return defriend_func && *defriend_func;
}

HNetworkAddressManagerPrivate::HNetworkAddressManagerPrivate(Ipv4Address_Type address, Ipv4Port_Type port)
	:ipv4Addr(address)
	,ipv4Ports({port})
{

}

HNetworkAddressManagerPrivate::HNetworkAddressManagerPrivate()
{

}

NAMESPACE_END_CHE