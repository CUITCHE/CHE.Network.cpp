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

HPacket& HPacket::read(const HDataBuffer& d)
{
	*data = d;
	read_impl();
	return *this;
}

HDataBuffer& HPacket::write()
{
	data->clear();

	write_impl();

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