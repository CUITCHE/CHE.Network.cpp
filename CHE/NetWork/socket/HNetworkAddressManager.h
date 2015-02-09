#ifndef HNetworkAddressManager_H__
#define HNetworkAddressManager_H__
//
/********************************************************************
	created:	2014/10/13
	created:	13:10:2014   9:35
	file ext:	hpp
	author:		CHE

	purpose:	ip��ַ�࣬����ip��ɣ�����ͬһip�µ����Բ�ͬ�˿ڵ�����
				һ��ipһ��������������ͻ����Defriend_Function��������
				���ϲ㷢�����ӶϿ��ź�
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

	/*����ip��port�Ĺ��캯��*/
	HNetworkAddressManager(Ipv4Address_Type address, Ipv4Port_Type port, HObject *parent = 0);

	/*�ƶ����캯��*/
	HNetworkAddressManager(HNetworkAddressManager &&rhs);

	/*�ƶ����ƺ���*/
	void operator=(HNetworkAddressManager &&rhs);

	~HNetworkAddressManager();

	/*�жϵ�ǰip�����Ƿ�Ϊ��,true:is empty, otherwise is not empty*/
	bool empty()const;

	/*����ip���͵�ַ*/
	Ipv4Address_Type toIpv4Address()const;

	/*port�˿������ӣ�true������false*/
	bool contain(Ipv4Port_Type port)const;

	/*portʧȥ����*/
	void disconnected(Ipv4Port_Type port);

	/*port����,�����������ʱ����������false*/
	bool connected(Ipv4Port_Type port);

	/*operator bool()���ش�ip�Ƿ�����������У�Ϊfalse��ʾ�����������*/
	operator bool()const;

	/*�����Ƿ�����,false����*/
	bool permit(Authentication mission);

	/*���ý����������ʱ������ ��λ������*/
	static bool setLimitTime(const int limit_t);

	/*���ý��������ʱ���õĻص�������
	 ����Ѿ����ù��ص����������ᷢ��ʲô*/
	void setDefriendFuncCallback(Defriend_Function &&func);
private:
	unique_ptr<HNetworkAddressManagerPrivate> d_ptr;
};

NAMESPACE_END_CHE
#endif // HIpv4Address_H__
