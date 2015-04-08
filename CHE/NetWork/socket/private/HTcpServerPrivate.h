#include "HThread.h"
using std::shared_ptr;
using std::function;
CHE_NAMESPACE_BEGIN

class HTcpSocket;
class HHostAddress;
class HNetworkDefender;
typedef std::function<void(shared_ptr<HTcpSocket> peersocket)> Socket_Pending_Function;

CHE_PRIVATE_DEFINED(HTcpServer)
{
	
	HTcpServerPrivate();
	uint32 listen_shutdown : 1;						//ֹͣ����
	SOCKET listen_fd = ~0;							//�����׽���4
	HNetworkDefender *net_defender = nullptr;		//�������4
	HHostAddress *local_address = nullptr;			//����󶨵ĵ�ַ24
	Socket_Pending_Function *socket_pending_func = nullptr;	//�µ����ӵ����ص�����24
};
CHE_NAMESPACE_END
