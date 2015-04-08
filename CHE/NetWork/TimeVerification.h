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

CHE_NAMESPACE_BEGIN

class Timeverification final
{
	Timeverification() = delete;
public:
	static void addServerURL(const char *url);
	static HDateTime getTime(int UTC = 8);
};
CHE_NAMESPACE_END
#endif // TimeVerification_H__
