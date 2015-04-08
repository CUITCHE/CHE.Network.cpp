#include "HPacket.h"
#include "htime.h"


const int __length__ = 8;
CHE_NAMESPACE_BEGIN
HPacket::HPacket(const ProtocolType::_ProtocolType _type /*= ProtocolType::THISISEMPTY*/)
	:type(_type), data(new HDataBuffer(__length__))
{
	
}

HPacket::~HPacket()
{
	delete data;
}

HPacket& HPacket::Read(const HDataBuffer& d)
{
	*data = d;
	//��ȡ����ͷ��
	readHeader();
	read();
	return *this;
}

HDataBuffer& HPacket::Write()
{
	data->clear();
	//д������ͷ��
	writeHeader();
	//д��������
	write();
	//���md5�ַ���
	setTail();
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

inline char * HPacket::getCharData()
{
	return (char *)data->getData();
}

long long HPacket::getSendTime() const
{
	long long t = 0;
	t = HByteConvert::toType<long long>(data->getData());
	return t;
}

void HPacket::writeHeader()
{
	setSendTime();
}

void HPacket::readHeader()
{
}

void HPacket::setSendTime()
{
	long long t = HTime::currentTime();
	write_to_data(t);
}

void HPacket::setTail()
{
}

CHE_NAMESPACE_END