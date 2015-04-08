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

	getsetter(string&, name, Name)
	getsetter(string&, password, Password)
	getsetter(int, levels, Levels)
protected:
	void write();
	void read();
private:
	string name;
	string password;
	int levels = -1;
};
CHE_NAMESPACE_END
#endif // registerpacket_H__