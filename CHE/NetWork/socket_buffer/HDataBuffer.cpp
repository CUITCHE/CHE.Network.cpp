#include "HDataBuffer.h"
const int __length__ = 40;
CHE_NAMESPACE_BEGIN

HDataBuffer::HDataBuffer()
	:m_Wpos(__length__)
	, m_Rpos(__length__)
	,m_data(__length__, 0)
{
}

HDataBuffer::~HDataBuffer()
{
}
HDataBuffer::HDataBuffer(HDataBuffer & rhs)
	: m_Wpos(rhs.m_Wpos)
	, m_Rpos(rhs.m_Rpos)
	, m_data(rhs.m_data)
{
}
HDataBuffer::HDataBuffer(HDataBuffer && rhs)
	:m_Wpos(rhs.m_Wpos)
	,m_Rpos(rhs.m_Rpos)
	,m_data(move_quick(rhs.m_data))
{
}
HDataBuffer& HDataBuffer::operator=(const HDataBuffer &d)
{
	m_Wpos = d.m_Wpos;
	m_Rpos = d.m_Rpos;
	m_data = d.m_data;
	return *this;
}

void HDataBuffer::operator=(HDataBuffer && rhs)
{
	m_Wpos = rhs.m_Wpos;
	m_Rpos = rhs.m_Rpos;
	m_data = move_quick(rhs.m_data);
}

void HDataBuffer::putBytes(const byte *data, uint32 len)
{
	assert(data != NULL);//指针安全检查
	assert(len >= 0);//长度安全检查
	
	
	//memcpy(&m_data[0] + m_Wpos, data, sizeof(char)*len);
	m_data.insert(m_data.end(), data, data + len);
	m_Wpos += len;
}
const byte* HDataBuffer::fetchBytes(UINT len)const
{
	const byte *pBuf = &m_data[0] + m_Rpos;
	m_Rpos += len;
	return pBuf;
}
void setDataLength(HDataBuffer &rhs, const int length)
{
	rhs.m_data.reserve(__length__ + length);
}
CHE_NAMESPACE_END