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
	uint32 listen_shutdown : 1;						//停止监听
	SOCKET listen_fd = ~0;							//监听套接字4
	HNetworkDefender *net_defender = nullptr;		//网络防御4
	HHostAddress *local_address = nullptr;			//服务绑定的地址24
	Socket_Pending_Function *socket_pending_func = nullptr;	//新的连接到来回调函数24
};
CHE_NAMESPACE_END
