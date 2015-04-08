#include "HDateTime.h"
#include <ctime>
CHE_NAMESPACE_BEGIN

HDateTime::HDateTime(const HDate date /*= HDate()*/, const HTimeC time /*= HTimeC()*/)
:d_date(date)
,d_time(time)
{

}

che::HDateTime HDateTime::currentDateTime()
{
	time_t ti = ::time(0);
	tm _tm;
	localtime_s(&_tm, &ti);
	HDateTime dateTime;
	dateTime = HDate(
		(_year)_tm.tm_year
		, (_month)_tm.tm_mon
		, (_day)_tm.tm_mday);
	dateTime = HTimeC(
		(_hour)_tm.tm_hour
		, (_minu)_tm.tm_min
		, (_seco)_tm.tm_sec);
	return dateTime;
}

CHE_NAMESPACE_END
