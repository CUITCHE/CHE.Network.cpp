#ifndef HTimeC_H__
#define HTimeC_H__

/********************************************************************
created:	2014/11/26
created:	26:11:2014   16:14
file base:	HTimeC
author:		CHE

purpose:	时分秒的存储类，提供时间输出输入，暂不提供时间计算
*********************************************************************/
#include "hglobal.h"
NAMESPACE_BEGIN_CHE
//小时
typedef uint8 _hour;
//分钟
typedef uint8 _minu;
//秒
typedef uint8 _seco;

class HTimeC
{
public:
	HTimeC(_hour h = 0, _minu m = 0, _seco s = 0);
	~HTimeC(){}

	_hour hour()const;
	_minu minute()const;
	_seco second()const;

	bool hour(_hour h);
	bool minute(_minu m);
	bool second(_seco s);

	void operator=(const HTimeC &rhs) { d_time = rhs.d_time; }
private:
	uint32 d_time = 0;
};
NAMESPACE_END_CHE
#endif // Time_H__
