#include "HDate.h"

NAMESPACE_BEGIN_CHE

HDate::HDate(_year y, _month m, _day d)
{
	year(y), month(m), day(d);
}

che::_year HDate::year()const
{
	return (_year)(d_date & 0xFF0000);
}

_month HDate::month()const
{
	return (_month)(d_date & 0xFF00);
}

_day HDate::day()const
{
	return (_day)(d_date & 0xFF);
}

bool HDate::year(_year y)
{
	return y >= 1900 ? d_date |= ((uint32)y << 16), true : false;
}

bool HDate::month(_month m)
{
	return m >= 1 && m <= 12 ? d_date |= ((uint32)m << 8), true : false;
}

bool HDate::day(_day d)
{
	if (!(d >= 1 && d <= 31)) {
		return false;
	}

	//以31天为基准点1 3 5 7 8 10 12
	static _day dayArray[] = { 0,3,0,1,0,1,0,0,1,0,1,0 };
	dayArray[1] = this->isLeap() ? 2 : 3;
	
	if (!(31 - dayArray[this->month() - 1] >= d)) {
		return false;
	}
	d_date |= d;
	return true;
}

bool HDate::isLeap()
{
	_year y = year();
	return ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0);
}

HDate::WeekDay HDate::week()
{
	int m = this->month();
	int yer = this->year();
	if (m < 3) {
		yer -= 1;
		m += 12;
	}
	int c = yer / 100;
	int y = yer - 100 * c;
	int w = c / 4 - 2 * c + y + y / 4 + 26 * (m + 1) / 10 + this->day() - 1;
	//预防负数
	w = (w % 7 + 7) % 7;
	return (WeekDay)w;
}

NAMESPACE_END_CHE