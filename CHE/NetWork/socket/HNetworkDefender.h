#ifndef HNetworkDefender_H__
#define HNetworkDefender_H__
//
/********************************************************************
	created:	2014/10/13
	created:	13:10:2014   9:12
	file ext:	hpp
	author:		CHE
	
	purpose:	网络库tcp socket的防御类
				提供ip认证
*********************************************************************/
#include "HNetworkAddressManager.h"
#include <map>
#include <string>
#include <memory>
using std::map;
using std::pair;
using std::shared_ptr;
CHE_NAMESPACE_BEGIN
class HNetworkDefender : public HObject
{
	CHE_DISABLE_COPY(HNetworkDefender)
public:
	typedef unique_ptr<HNetworkAddressManager> Ipv4Address_Value_Type;
	typedef map<Ipv4Address_Type, Ipv4Address_Value_Type> Ipv4Address_Acount_Map;
public:
	HNetworkDefender(HObject *parent = 0);

	HNetworkDefender(HNetworkDefender &&rhs);

	void operator=(HNetworkDefender &&rhs);

	~HNetworkDefender();

	/*ip认证，认证成功返回true，否则返回false*/
	bool Ipv4AddressAuthenticate(Ipv4Address_Type ipAddress, Ipv4Port_Type port);

	/*退出服务。传入错误或者没有这个ip在map中 将返回false*/
	bool Ipv4AddressQuit(Ipv4Address_Type ipAddress, Ipv4Port_Type port);
private:
	unique_ptr<Ipv4Address_Acount_Map> ipv4Addre_accounts;		//维护所有连入的ip,key=ip,value=ip管理类
};
CHE_NAMESPACE_END
#endif // HNetworkDefender_H__
