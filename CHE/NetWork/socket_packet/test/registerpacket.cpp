#include "registerpacket.h"
CHE_NAMESPACE_BEGIN
RegisterPacket::RegisterPacket()
	:HPacket(HType::REGISTER)
{
}

void RegisterPacket::write()
{
	HPacket::write_to_data(name, password, levels);
}

void RegisterPacket::read()
{
	HPacket::read_from_data(name, password, levels);
}

CHE_NAMESPACE_END
