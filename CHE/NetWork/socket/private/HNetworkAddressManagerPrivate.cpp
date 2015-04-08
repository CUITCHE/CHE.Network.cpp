#include "../NetWork/socket/private/HNetworkAddressManagerPrivate.h"

CHE_NAMESPACE_BEGIN
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

CHE_NAMESPACE_END
