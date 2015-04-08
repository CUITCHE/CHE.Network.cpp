#ifndef HDateTime_H__
#define HDateTime_H__

/********************************************************************
	created:	2014/11/26
	created:	26:11:2014   15:43
	file base:	HDateTime
	author:		CHE
	
	purpose:	存储年月日时分秒的类。不提供日期计算
*********************************************************************/
#include "hglobal.h"
#include "HTimeC.h"
#include "HDate.h"
CHE_NAMESPACE_BEGIN
class HDateTime
{
public:
	HDateTime(const HDate date = HDate(), const HTimeC time = HTimeC());
	~HDateTime(){}
	_year year()const { return d_date.year(); }
	void year(_year y) { d_date.year(y); }

	_month month()const { return d_date.month(); }
	void month(_month m) { d_date.month(m); }

	_day day()const { return d_date.month(); }
	void day(_day d) { d_date.day(d); }

	_hour hour()const { return d_time.hour(); }
	void hour(_hour h) { d_time.hour(h); }

	_minu minute()const { return d_time.minute(); }
	void minute(_minu m) { d_time.minute(m); }

	_seco second()const { return d_time.second(); }
	void second(_seco s) { d_time.second(s); }

	void operator=(const HDate& date) { d_date = date; }
	void operator=(const HTimeC& time) { d_time = time; }

	HDate date()const { return d_date; }
	HTimeC time()const { return d_time; }

	//获取本机当前时间
	static HDateTime currentDateTime();
private:
	HDate d_date;
	HTimeC d_time;
};
CHE_NAMESPACE_END
#endif // HDateTime_H__
