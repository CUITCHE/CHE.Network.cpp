#ifndef TimeVerification_H__
#define TimeVerification_H__

/********************************************************************
	created:	2014/11/17
	created:	17:11:2014   12:45
	file base:	TimeVerification
	author:		CHE
	
	purpose:	时间验证服务
*********************************************************************/
#include "HDateTime.h"

NAMESPACE_BEGIN_CHE

class Timeverification final
{
	Timeverification() = delete;
public:
	static void addServerURL(const char *url);
	static HDateTime getTime(int UTC = 8);
};
NAMESPACE_END_CHE
#endif // TimeVerification_H__
