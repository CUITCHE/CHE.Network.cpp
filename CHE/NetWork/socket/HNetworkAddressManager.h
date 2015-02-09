#ifndef HNetworkAddressManager_H__
#define HNetworkAddressManager_H__
//
/********************************************************************
	created:	2014/10/13
	created:	13:10:2014   9:35
	file ext:	hpp
	author:		CHE

	purpose:	ip地址类，管理ip许可，管理同一ip下的来自不同端口的连接
				一个ip一旦进入黑名单，就会调用Defriend_Function函数对象
				向上层发送连接断开信号
	*********************************************************************/
#include "../NetWork/socket/private/HNetworkAddressManagerPrivate.h"
#include "htime.h"

NAMESPACE_BEGIN_CHE
class HNetworkAddressManager :public HObject
{
	CHE_PRIVATE_DECLARE(HNetworkAddressManager)
	CHE_DISABLE_COPY(HNetworkAddressManager)
public:
	HNetworkAddressManager(HObject *parent = 0);

	/*传入ip和port的构造函数*/
	HNetworkAddressManager(Ipv4Address_Type address, Ipv4Port_Type port, HObject *parent = 0);

	/*移动构造函数*/
	HNetworkAddressManager(HNetworkAddressManager &&rhs);

	/*移动复制函数*/
	void operator=(HNetworkAddressManager &&rhs);

	~HNetworkAddressManager();

	/*判断当前ip网络是否为空,true:is empty, otherwise is not empty*/
	bool empty()const;

	/*返回ip整型地址*/
	Ipv4Address_Type toIpv4Address()const;

	/*port端口已连接：true，否则false*/
	bool contain(Ipv4Port_Type port)const;

	/*port失去连接*/
	void disconnected(Ipv4Port_Type port);

	/*port连入,如果低于连入时间间隔，返回false*/
	bool connected(Ipv4Port_Type port);

	/*operator bool()返回此ip是否被拉入黑名单中，为false表示已拉入黑名单*/
	operator bool()const;

	/*设置是否拉黑,false拉黑*/
	bool permit(Authentication mission);

	/*设置进入黑名单的时间限制 单位：毫秒*/
	static bool setLimitTime(const int limit_t);

	/*设置进入黑名单时调用的回调函数，
	 如果已经设置过回调函数将不会发生什么*/
	void setDefriendFuncCallback(Defriend_Function &&func);
private:
	unique_ptr<HNetworkAddressManagerPrivate> d_ptr;
};

NAMESPACE_END_CHE
#endif // HIpv4Address_H__
