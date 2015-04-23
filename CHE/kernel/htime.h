#ifndef htime_H__
#define htime_H__
/********************************************************************
	created:	2014/10/08
	created:	8:10:2014   18:45
	file ext:	h
	author:		CHE
	
	purpose:	公共平台上只提供简单的时间time_t数值返回，和fmt时间
				Windows平台上提供高效的高精度——纳秒级别的时间计时
				不提供时间计算，若想要时间计算，请用class Time
*********************************************************************/
#include "hglobal.h"
#include "HDateTime.h"
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
using std::vector;
using std::string;
using std::shared_ptr;
using namespace std::chrono;

union _LARGE_INTEGER;

CHE_NAMESPACE_BEGIN

class HTime
{
	friend class Timeverification;
public:
	//用于表示要输出的精度，程序默认为毫秒级
	enum OutputPrecision
	{
		//秒
		Normalsecond,
		//毫秒
		Millisecond,
		//微秒
		Microsecond,
		//纳秒
		Nanosecond
	};
#ifdef WIN32
	HTime(OutputPrecision precision = Millisecond);
	~HTime();
	//开始计时
	void start();
	//重新开始计时
	void restart();
	//计算从start或者restart被调用后的时间间隔
	long double elapsed();
protected:
	//设置精度相关参数
	inline void setPrecision(OutputPrecision precision);
private:
	_LARGE_INTEGER *tStart, *tEnd;
	OutputPrecision outputPrecision;
	double precisionNum;	//为了表示精度，要除以的数，以秒为基本单位
	long long tFreq;	//表示CPU频率
#else
public:
	HTime(OutputPrecision precision = Millisecond){}
	~HTime(){}
	//开始计时
	void start(){}
	//重新开始计时
	void restart(){}
	//计算从start或者restart被调用后的时间间隔
	long double elapsed(){}
#endif // WIN32

public:
	//获得当前系统时间以time_t返回，精确到毫秒
	static time_t currentTime();

	//获得日期 2014/02/12 传入的string会替换日期例子中的"/"
	static string currentDate(char type = '/');

	//获得时间 12:02:13，传入true返回12小时制，false返回24小时制
	static string& currentTime(bool _in);

	//获得日期和时间，日期为固定两位，24小时制
	static string currentDateTime(char type = '_');

	//返回“2014/11/30 14:05:30 星期日”的string
	static string fmt_currentDateTime();

protected:
	//获得网络UTC时间，传入utc，时区差
	//返回一个把年月日时分秒，每个值1个字节存储在uint64的低48位中
	//利用year, month,day等函数提取出具体的值，也可以合成这样的日期时间
	static HDateTime getNetUTCTime(int UTC = 8);
private:
	static vector<string> server_name;
};
CHE_NAMESPACE_END
#endif // htime_H__
