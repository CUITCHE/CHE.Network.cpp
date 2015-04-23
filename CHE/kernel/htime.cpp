#include "htime.h"
#include "HHostAddress.h"
#include "HLog.h"
CHE_NAMESPACE_BEGIN

vector<string> HTime::server_name{ "203.117.180.36", "131.107.1.10", "202.120.2.101" };

#ifdef WIN32
#include <windows.h>
HTime::HTime(OutputPrecision precision /*= NormalSecond*/)
	:tStart(new _LARGE_INTEGER),
	tEnd(new _LARGE_INTEGER)
{
	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	tFreq = tmp.QuadPart;

	setPrecision(precision);
}

HTime::~HTime()
{
	delete tStart;
	delete tEnd;
}

void HTime::setPrecision(OutputPrecision precision)
{
	outputPrecision = precision;

	switch (outputPrecision)
	{
	case Normalsecond:
		precisionNum = 1.0/tFreq;
		break;
	case Millisecond:
		precisionNum = 1000.0/tFreq;
		break;
	case Microsecond:
		precisionNum = 1000000.0 / tFreq;
		break;
	case Nanosecond:
		precisionNum = 1000000000.0 / tFreq;
		break;
	}
}

void HTime::start()
{
	QueryPerformanceCounter(tStart);
}

void HTime::restart()
{
	QueryPerformanceCounter(tStart);
}

long double HTime::elapsed()
{
	QueryPerformanceCounter(tEnd);
	return (tEnd->QuadPart - tStart->QuadPart) * precisionNum;
}
#endif // WIN32
string& HTime::currentTime(bool _in)
{
	time_t timeData;
	timeData = time(0);
	tm _tm;
	localtime_s(&_tm, &timeData);
	char strTime[128];
	_in && _tm.tm_hour > 12 ? _tm.tm_hour -= 12 : 0;
	sprintf_s(strTime, "%02d:%02d:%02d", _tm.tm_hour, _tm.tm_min, _tm.tm_sec);

	return (string(strTime));
}

time_t HTime::currentTime()
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(t1.time_since_epoch()).count();
}

HDateTime HTime::getNetUTCTime(int UTC /*= 8*/)
{
	WSADATA wsd;
	if (WSAStartup(0x0202, &wsd) < 0){
		Warning("socket init failed!");
		return 0;
	}
	SOCKET fdt = INVALID_SOCKET;
	fdt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	char _recv[100];
	HDateTime res;
	CheZeroMemory(_recv, sizeof(_recv));
	auto iter_b = server_name.begin();
	auto iter_e = server_name.end();
	while (iter_b != iter_e){
		HHostAddress abc(iter_b->c_str(), 13);
		if (connect(fdt, abc.sockAddr(), sizeof(sockaddr)) == SOCKET_ERROR){
			logger->warningLog("% connection failed! code:%", abc.toString(), GetLastError());
			++iter_b;
			continue;
		}
		recv(fdt, _recv, 100, 0);
		char *ptr = _recv;
		//找到第一个空格
		while (*++ptr != ' ')	continue;
		++ptr;

		//年
		*(ptr + 2) = 0;
		auto _6 = (_year)atoi(ptr);
		ptr += 3;

		//月
		*(ptr + 2) = 0;
		auto _5 = (_month)atoi(ptr);
		ptr += 3;

		//日
		*(ptr + 2) = 0;
		auto _4 = (_day)atoi(ptr);
		ptr += 3;

		//时
		*(ptr + 2) = 0;
		auto _3 = (_hour)(atoi(ptr) + UTC);
		ptr += 3;

		//分
		*(ptr + 2) = 0;
		auto _2 = (_minu)atoi(ptr);
		ptr += 3;

		//秒
		*(ptr + 2) = 0;
		auto _1 = (_seco)atoi(ptr);
		ptr += 3;

		res = HDate(_6, _5, _4);
		res = HTimeC(_3, _2, _1);
		break;
	}
	closesocket(fdt);
	WSACleanup();
	return res;
}

string HTime::fmt_currentDateTime()
{
	static char *xq[] = { "星期日","星期一","星期二","星期三","星期四","星期五","星期六" };
	time_t timeData;
	timeData = time(0);
	tm _tm;
	localtime_s(&_tm, &timeData);
	char strTime[128];
	sprintf_s(strTime, "%4d/%02d/%02d %02d:%02d:%02d %s", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday
		,_tm.tm_hour,_tm.tm_min,_tm.tm_sec,xq[_tm.tm_wday]);

	return string(strTime);
}

string HTime::currentDate(char type /*= '/'*/)
{
	time_t timeData;
	timeData = time(0);
	tm _tm;
	localtime_s(&_tm, &timeData);
	char strTime[128];
	sprintf_s(strTime, "%4d-%02d-%02d", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday);
	string timeString = strTime;
	
	if (type == '-') return move_quick(timeString);

	timeString[4] = type;
	timeString[7] = type;

	return (timeString);
}

string HTime::currentDateTime(char type /*= '_'*/)
{
	time_t timeData;
	timeData = time(0);
	tm _tm;
	localtime_s(&_tm, &timeData);
	char strTime[128];
	sprintf_s(strTime, "%4d_%02d_%02d_%02d_%02d_%02d", _tm.tm_year + 1900, _tm.tm_mon + 1,
		_tm.tm_mday,_tm.tm_hour, _tm.tm_min, _tm.tm_sec);
	string timeString = strTime;

	if (type == '_') return move_quick(timeString);

	timeString[4]  = type;
	timeString[7]  = type;
	timeString[10] = type;
	timeString[13] = type;
	timeString[16] = type;

	return (timeString);
}
CHE_NAMESPACE_END
