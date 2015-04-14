#include "registerpacket.h"
CHE_NAMESPACE_BEGIN
RegisterPacket::RegisterPacket()
	:HPacket(HType::REGISTER)
{
}

void RegisterPacket::write_impl()
{
	HPacket::write_to_data(name, password, levels, _short, _byte, ___int64, _double, _float);
}

void RegisterPacket::read_impl()
{
	HPacket::read_from_data(name, password, levels, _short, _byte, ___int64, _double, _float);
}

CHE_NAMESPACE_END
