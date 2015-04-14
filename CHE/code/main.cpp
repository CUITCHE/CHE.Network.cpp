#include "HByteConvert.h"
#include "registerpacket.h"
#include "HDebug.hpp"
#include <vector>
#include "HTimer.h"
using namespace std;
using namespace che;
namespace che {
	class ABC :public HObject
	{
	public:
		void exec1() {
			fprintf_s(stderr, "echo....\n");
		}
	};
}
void exec1() {
	while (1)
	{
		__sleep(2);
		continue;
	}
}
int main()
{
	ABC *a = new ABC;
	HTimer *timer = new HTimer(&ABC::exec1, a);
	timer->start(500);
	thread t1(exec1);
	t1.join();
	return 0;
}
