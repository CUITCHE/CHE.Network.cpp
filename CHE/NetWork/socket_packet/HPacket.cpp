
#include "HPacket.h"
#include <cassert>
NAMESPACE_BEGIN_CHE
HPacket::HPacket(ProtocolType::_ProtocolType _type /*= ProtocolType::THISISEMPTY*/)
:data(_type)
{

}

HPacket::~HPacket()
{
}

HPacket& HPacket::Read(HDataBuffer& d)
{
	data = d;	//�������ȸ��Ʋ�������
	assert(_read);
	_read();
	return *this;
}

HDataBuffer& HPacket::Write()
{
	data.clearALL();
	assert(_write);
	_write();
	data.writeOtherInfo();
	return data;
}
NAMESPACE_END_CHE