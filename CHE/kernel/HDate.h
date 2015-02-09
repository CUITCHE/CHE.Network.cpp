#ifndef HDate_H__
#define HDate_H__

/********************************************************************
	created:	2014/11/26
	created:	26:11:2014   16:50
	file base:	HDate
	author:		CHE
	
	purpose:	���ڣ������յĴ洢�࣬���ṩ���ڼ���
				�ṩ�����������
				�꣺1900-65536
				�£�1-12
				�գ�1-31�������·�Ӱ�죩
*********************************************************************/
#include "hglobal.h"
NAMESPACE_BEGIN_CHE
//��
typedef uint16 _year;
//��
typedef uint8 _month;
//��
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

	//�ж���һ���Ƿ������꣬if it is leap year, return true;otherwise false
	bool isLeap();

	//�������ڵ�����
	HDate::WeekDay week();
private:
	uint32 d_date = 0;
};
NAMESPACE_END_CHE
#endif // HDate_H__
