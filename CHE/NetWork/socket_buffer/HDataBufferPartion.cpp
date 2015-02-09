//
#include "HDataBufferPartion.h"
#include "HByteConvert.h"
#include <cassert>
NAMESPACE_BEGIN_CHE

//boost::pool<> memory32byte(sizeof(byte)*EachPacketSize);

HDataBufferPartion::HDataBufferPartion(const byte* head, const byte* data, size_t secParam_size)
{
	m_dataPartBegin = new byte[32];
	errno_t err = memcpy_s(m_dataPartBegin, sizeof(byte)*DATA_BEGIN, head, sizeof(byte)*DATA_BEGIN);
	assert(err == 0);
	err = memcpy_s(m_dataPartBegin + DATA_BEGIN, sizeof(byte)*secParam_size, data, sizeof(byte)*secParam_size);
	assert(err == 0);
}

HDataBufferPartion::~HDataBufferPartion()
{
}

int64 HDataBufferPartion::getGUID()
{
	return HByteConvert::toType<long long>(m_dataPartBegin + PACKETGUID_BEGIN);
}

int16 HDataBufferPartion::getType()
{
	return HByteConvert::toType<short>(m_dataPartBegin + PROTOCOL_BEGIN);
}

int32 HDataBufferPartion::getSerial()
{
	return (int32)(HByteConvert::toType<long long>(m_dataPartBegin + PACKETSERIAL_BEGIN) & 0xFFFFFFFFL);
}

int32 HDataBufferPartion::getPacketTotall()
{
	return (int32)(HByteConvert::toType<long long>(m_dataPartBegin + PACKETSERIAL_BEGIN) >> 32);
}

NAMESPACE_END_CHE