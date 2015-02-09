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
	Authentication authorization = 1;		//����Ϊtrue������Ϊfalse
	Ipv4Address_Type ipv4Addr = 0;			//ip��ַ
	time_t recent_connected_time = 0;		//���һ�����ϵ�ʱ��
	Defriend_Function *defriend_func = 0;	//���ڵ�ʱ�򣬽����������ص�������������ڵĻ�
	list<Ipv4Port_Type> ipv4Ports;			//�����ip��ַ�µ��������Ӷ˿�

	static int s_time_limit;

	/*�жϻص���������Ч�ԣ���Ч����true�����򷵻�false*/
	bool isDefriendFunctionCallbackValid() const;
};
NAMESPACE_END_CHE