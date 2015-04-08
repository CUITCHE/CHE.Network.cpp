#include "HDataBuffer.h"
CHE_NAMESPACE_BEGIN

HDataBuffer::HDataBuffer(uint32 writePosition)
	:m_Wpos(writePosition-1), m_Rpos(writePosition-1)
	, packetBeginPosition(writePosition)
{
	m_data.resize(packetBeginPosition);

}

HDataBuffer::~HDataBuffer()
{
}
HDataBuffer::HDataBuffer(HDataBuffer & rhs)
	:packetBeginPosition(rhs.packetBeginPosition)
	, m_Wpos(rhs.m_Wpos)
	, m_Rpos(rhs.m_Rpos)
	, m_data(rhs.m_data)
{
}
HDataBuffer::HDataBuffer(HDataBuffer && rhs)
	:packetBeginPosition(rhs.packetBeginPosition)
	,m_Wpos(rhs.m_Wpos)
	,m_Rpos(rhs.m_Rpos)
	,m_data(move_quick(rhs.m_data))
{
}
HDataBuffer& HDataBuffer::operator=(const HDataBuffer &d)
{
	packetBeginPosition = d.packetBeginPosition;
	m_Wpos = d.m_Wpos;
	m_Rpos = d.m_Rpos;
	m_data = d.m_data;
	return *this;
}

void HDataBuffer::operator=(HDataBuffer && rhs)
{
	packetBeginPosition = rhs.packetBeginPosition;
	m_Wpos = rhs.m_Wpos;
	m_Rpos = rhs.m_Rpos;
	m_data = move_quick(rhs.m_data);
}

void HDataBuffer::putBytes(const byte *data, uint32 len)
{
	assert(data != NULL);//指针安全检查
	assert(len >= 0);//长度安全检查
	
	
	m_data.resize(m_Wpos+len);
	memcpy(&m_data[0] + m_Wpos, data, sizeof(byte)*len);

	m_Wpos += len;
}
const byte* HDataBuffer::fetchBytes(UINT len)const
{
	const byte *pBuf = &m_data[0] + m_Rpos;
	m_Rpos += len;
	return pBuf;
}
CHE_NAMESPACE_END