#include "HObject.h"
#include <list>
#include <algorithm>

using std::list;
using std::function;

NAMESPACE_BEGIN_CHE

class HNetworkAddressManager;
typedef uint32 Ipv4Address_Type;
typedef uint16 Ipv4Port_Type;
typedef bool Authentication;
typedef function<void(Ipv4Address_Type blackAddress)> Defriend_Function;

CHE_PRIVATE_DEFINED(HNetworkAddressManager)
{
	HNetworkAddressManagerPrivate();
	HNetworkAddressManagerPrivate(Ipv4Address_Type address, Ipv4Port_Type port);
	Authentication authorization = 1;		//正常为true，拉黑为false
	Ipv4Address_Type ipv4Addr = 0;			//ip地址
	time_t recent_connected_time = 0;		//最近一次连上的时间
	Defriend_Function *defriend_func = 0;	//拉黑的时候，将会调用这个回调函数，如果存在的话
	list<Ipv4Port_Type> ipv4Ports;			//在这个ip地址下的所有连接端口

	static int s_time_limit;

	/*判断回调函数的有效性，有效返回true，否则返回false*/
	bool isDefriendFunctionCallbackValid() const;
};
NAMESPACE_END_CHE