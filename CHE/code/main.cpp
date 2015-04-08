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
#include "HByteConvert.h"
using namespace std;
using namespace che;
template<typename T>
void write_t(T d) {
	cout << d;
}
template<>
void write_t<double>(double d) {
	printf("%.2lf----\n", d);
}
int main()
{
	vector<int> abc{ 1,2,3,5,4,8,7,8,6,5,4,8 };
	HDataBuffer data(32);
	HByteConvert::write(data, abc);
	return 0;
}