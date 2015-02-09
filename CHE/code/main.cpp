#include "HLog.h"
#include "HIOCP.h"
#include "htime.h"
#include "HighEffectIOBuffer.h"
#include "HTcpServer.h"
#include <memory>
#include <thread>
#include <fstream>
#include <atomic>
#include "HSlice.h"
using namespace std;
using namespace che;

atomic_int ncount = 0;
shared_ptr<HTcpSocket> g_sock;
void exec(unique_ptr<HighEffectIOBuffer> &buffer) {
	NetIOBuffer tmp;
	time_t time = ::time(0);
	
	forever{
		if (buffer->nextpadding() == false) {
			this_thread::sleep_for(chrono::milliseconds(1));
			if (::time(0) - time > 5) {
				time = ::time(0);
				logger->warningLog( "size of unsued:", buffer->unused_size(), "received:", (int)ncount);
			}
			continue;
		}
		time = ::time(0);
		buffer->buffer_used(&tmp);
		//TODO：对tmp的buffer做一些处理，这里就输出吧
		logger->warningLog( tmp.buf());
		++ncount;
		buffer->buffer_used(move_quick(tmp));
	}
}
void eeeexe(shared_ptr<HTcpSocket> obj) {

	string str="23556";
	forever{
		obj->write(str);
		__sleep(1);
	}
}
void mayPrint(const char *buffer, unsigned int len) {
	char *str = new char[len + 1];
	memcpy(str, buffer, len);
	str[len] = 0;
	printf_s("%s\n", str);
}
void pending(shared_ptr<HTcpSocket> sock) {
	g_sock = move_quick(sock);

	Recv_Tcp_Function func = std::bind(mayPrint, placeholders::_1, placeholders::_2);
	g_sock->setRecvFunction(move_quick(func));
	thread *th1 = new thread(std::bind(eeeexe, g_sock));
}

int main()
{
	char *str = "abcdefghiljk";
	HSlice<int> a = { 1,2,3,4,5,6,7,8,9,10 };
	cout << (a[{-1,8}] = { 11,12,13,14,15 }) << endl;
	cout << a << endl;
	return 0;
}