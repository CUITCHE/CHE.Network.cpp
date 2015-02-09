#ifndef HDate_H__
#define HDate_H__

/********************************************************************
	created:	2014/11/26
	created:	26:11:2014   16:50
	file base:	HDate
	author:		CHE
	
	purpose:	日期：年月日的存储类，不提供日期计算
				提供日期输入输出
				年：1900-65536
				月：1-12
				日：1-31（会受月份影响）
*********************************************************************/
#include "hglobal.h"
NAMESPACE_BEGIN_CHE
//年
typedef uint16 _year;
//月
typedef uint8 _month;
//日
typedef uint8 _day;

class HDate
{
public:
	enum WeekDay {
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};
	HDate(_year y = 1900, _month m = 1, _day d= 1);
	~HDate() {};
	_year year()const;
	_month month()const;
	_day day()const;

	bool year(_year y);
	bool month(_month m);
	bool day(_day d);

	void operator=(const HDate &rhs) { d_date = rhs.d_date; }

	//判断这一年是否是闰年，if it is leap year, return true;otherwise false
	bool isLeap();

	//计算日期的星期
	HDate::WeekDay week();
private:
	uint32 d_date = 0;
};
NAMESPACE_END_CHE
#endif // HDate_H__
