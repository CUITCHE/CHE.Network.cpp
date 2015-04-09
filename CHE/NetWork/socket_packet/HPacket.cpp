#include "HPacket.h"
#include "htime.h"



CHE_NAMESPACE_BEGIN
HPacket::HPacket(const ProtocolType::_ProtocolType _type /*= ProtocolType::THISISEMPTY*/)
	:type(_type), data(new HDataBuffer)
{
	
}

HPacket::~HPacket()
{
	delete data;
}

HPacket& HPacket::Read(const HDataBuffer& d)
{
	*data = d;
	read();
	return *this;
}

HDataBuffer& HPacket::Write()
{
	data->clear();

	write();

	return *data;
}

inline uint32 HPacket::getSize() const
{
	return data->getDataLength();
}

inline void HPacket::setData(const HDataBuffer & d)
{
	*data = d;
}

inline const HDataBuffer & HPacket::getData() const
{
	return *data;
}
inline const char * HPacket::toLatin() const
{
	return (const char *)data->data();
}
CHE_NAMESPACE_END