#ifndef registerpacket_H__
#define registerpacket_H__

/********************************************************************
	created:	2014/10/11
	created:	11:10:2014   15:25
	file ext:	h
	author:		CHE
	
	purpose:	
*********************************************************************/
#include "HPacket.h"
CHE_NAMESPACE_BEGIN
class RegisterPacket :public HPacket
{
public:
	RegisterPacket();
	~RegisterPacket(){}

	GETSETTER(string, name, Name);
	GETSETTER(string, password, Password);
	GETSETTER(int, levels, Levels);
	GETSETTER(short, _short, _short);
	GETSETTER(byte, _byte, _byte);
	GETSETTER(long long, ___int64, ___int64);
	GETSETTER(double, _double, _double);
	GETSETTER(float, _float, _float);
protected:
	void write();
	void read();
};
CHE_NAMESPACE_END
#endif // registerpacket_H__