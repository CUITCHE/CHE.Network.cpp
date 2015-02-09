//
#include "registerpacket.h"
NAMESPACE_BEGIN_CHE
RegisterPacket::RegisterPacket(void)
	:HPacket(HType::REGISTER)
{
	bind(this, &RegisterPacket::write, &RegisterPacket::read);
}

RegisterPacket::RegisterPacket(string _name, string pw)
	:HPacket(HType::REGISTER),name(_name),password(pw)
{
	bind(this, &RegisterPacket::write, &RegisterPacket::read);
}

void RegisterPacket::write()
{
	HPacket::write_to_data(name, password, levels);
}

void RegisterPacket::read()
{
	HPacket::read_from_data(name, password, levels);
}

NAMESPACE_END_CHE