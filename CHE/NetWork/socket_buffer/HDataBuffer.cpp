//
#include "HDataBuffer.h"
#include "htime.h"
#include "HByteConvert.h"
#include <cstring>
#include <stack>
#include <iostream>
#include <cmath>
#include <cassert>
NAMESPACE_BEGIN_CHE
using std::stack;

HDataBuffer::HDataBuffer(BufferPartionSet &bufSet,  bool &ok)
{
	if (bufSet.size() == 0)
	{
		ok = false;
		return;
	}
	auto iter = bufSet.begin();
	if ((*iter)->getSerial() != 0)
	{
		ok = false;
		return;
	}
	ok = true;
	m_guid = (*iter)->getGUID();
	m_type = (*iter)->getType();
	m_timeCode = m_guid^m_type;
	m_packetSize = (EachPacketSize - DATA_BEGIN) * (bufSet.size() - 1) + EachPacketSize;
	m_Wpos = m_Rpos = PACKETDATA_BEGIN;
	readFromBufferSet(bufSet);
}

HDataBuffer::HDataBuffer(int16 type)
	:m_type(type), m_Wpos(PACKETDATA_BEGIN), m_Rpos(PACKETDATA_BEGIN),
	m_packetSize(DATA_BEGIN), m_timeCode(HTime::currentTime()), m_guid(m_timeCode^m_type)
{
	//写入包的数据从标号0开始
	m_data.resize(DATA_BEGIN);
	//guid写入
	const byte *pBuf = HByteConvert::toBytes(m_guid);
	memcpy(&guidBytesInfo[0] + PACKETGUID_BEGIN, pBuf, sizeof(byte) * 8);
	memcpy(&m_data[0] + PACKETGUID_BEGIN, pBuf, sizeof(byte) * 8);
	delete []pBuf;
	//协议写入
	pBuf = HByteConvert::toBytes(m_type);
	memcpy(&guidBytesInfo[0] + PROTOCOL_BEGIN, pBuf, sizeof(byte) * 2);
	memcpy(&m_data[0] + PROTOCOL_BEGIN, pBuf, sizeof(byte) * 2);
	delete []pBuf;
}

HDataBuffer::~HDataBuffer()
{

}
HDataBuffer& HDataBuffer::operator=(const HDataBuffer &d)
{
	byte* pBuf = (const_cast<HDataBuffer &>(d)).getData();
	m_guid = HByteConvert::toType<long long>(pBuf);
	m_type = HByteConvert::toType<short>(pBuf + PROTOCOL_BEGIN);
	m_timeCode = m_guid^m_type;
	m_Wpos = m_Rpos = PACKETDATA_BEGIN;
	m_packetSize = d.getDataLength();
	m_data.resize(m_packetSize);//准备空间
	memcpy(&m_data[0],pBuf,m_packetSize);
	
	return *this;
}
bool HDataBuffer::buildFromByteArray(const byte* pBuf, uint32 size)
{

	if (size < DATA_BEGIN)	return false;//这里做了数据限制
	this->m_packetSize = size;

	m_guid = HByteConvert::toType<long long>(pBuf + PACKETGUID_BEGIN);
	m_type = HByteConvert::toType<short>(pBuf + PROTOCOL_BEGIN);
	this->m_timeCode = m_guid^m_type;
	if (m_timeCode <= 0) return false;
	

	m_Wpos = m_Rpos = PACKETDATA_BEGIN;
	m_data.resize(m_packetSize);//准备空间
	memcpy(&m_data[0],pBuf,m_packetSize);
	//这里传入的是byte*数组就不能保证是解密了，故这里要进行一个解密操作
//	decrypte();
//	m_type = HByteConvert::byteToShort(&m_data[0]+PROTOCOL_BEGIN);

	return true;
}
bool HDataBuffer::getPacketSizeFromByteArry(const byte *pBuf)
{
// 	uint32 packetSize = HByteConvert::byteToInt(pBuf+PACKET_BEGIN);
// 	if (packetSize <= 0 || packetSize > 1024 * 95)	return false;//这里做了数据限制，一旦超过了80KB，就不做处理
// 	this->setPacketSize(packetSize);
	return true;
}

bool HDataBuffer::getTimeCheckFromByteArry(const byte *pBuf)
{
// 	int64 timeCode = HByteConvert::byteToLong64(pBuf + CHECKCODE_BEGIN);
// 	if (timeCode <= 0) return false;
// 	this->setTimeCheck(timeCode);
	return true;
}
void HDataBuffer::putBytes(const byte *data, uint32 len)
{
	assert(data != NULL);//指针安全检查
	assert(len >= 0);//长度安全检查
	
	m_data.resize(m_Wpos+len);
	memcpy(&m_data[0] + m_Wpos, data, sizeof(byte)*len);

	m_Wpos += len;
}
const byte* HDataBuffer::fetchBytes(UINT len)
{
// 	Q_ASSERT_X(len >= 0, "", "字节包写入的数据的长度不为正数！");//长度安全检查
// 	byte *pBuf = new byte[len + 1];
// 	pBuf[len] = 0;
// 	Q_ASSERT_X(pBuf != NULL, "","fetchBytes()内存申请错误！");//安全检查
// 
// 	memcpy(pBuf, &m_data[0] + m_Rpos, sizeof(byte)*len);
	const byte *pBuf = &m_data[0] + m_Rpos;
	m_Rpos += len;
	return pBuf;
}
void HDataBuffer::writeOtherInfo()
{

	m_packetSize = m_Wpos;
//	int nums = m_packetSize / EachPacketSize + 1;
	//进行拆包工作
	double partionSizedb = (double)(m_packetSize - DATA_BEGIN) / PACKETFETCHNUMS;
	long long bufferPartionSize = (long long)ceil(partionSizedb);		//总分包的个数
	bufferPartionSize <<= 32;		//高32位表示包的总数，低32位表示当前包序号
	//每次从m_data中提取PACKETFETCHNUMS个字节数据
	byte *pData = NULL;
	int ret = 0;
	m_Fpos = PACKETDATA_BEGIN;		//提取数据的起始地址偏移量
	while ((ret = tryFetchPACKETFETCHNUMSBytes(pData)) != 0)
	{
		//写入包序号
		const byte *pBuf = HByteConvert::toBytes(bufferPartionSize++);
		memcpy(&guidBytesInfo[0] + PACKETSERIAL_BEGIN, pBuf, sizeof(byte) * 8);
		delete[]pBuf;
		shared_ptr<HDataBufferPartion> tmpBufferPartion(new HDataBufferPartion(guidBytesInfo, pData, ret));
		m_bufferSet.push_back(tmpBufferPartion);
	}
}

int32 HDataBuffer::tryFetchPACKETFETCHNUMSBytes(byte* &data)
{
	data = NULL;
	int ret = 0;
	if (m_packetSize - m_Fpos >= PACKETFETCHNUMS)
	{
		data = &m_data[0] + m_Fpos;
		ret = PACKETFETCHNUMS;
		m_Fpos += PACKETFETCHNUMS;
	}
	else if (m_packetSize - m_Fpos > 0)
	{
		data = &m_data[0] + m_Fpos;
		ret = m_packetSize - m_Fpos;
		m_Fpos = m_packetSize;
	}
	
	return ret;
}

void HDataBuffer::readFromBufferSet(BufferPartionSet &sets)
{
	if (sets.size() == 0)	return;
	m_data.clear();
	auto iter = sets.begin();
	//包数量验证
	assert((*iter)->getPacketTotall() == sets.size());
	m_data.resize(EachPacketSize);
	//第一包的读取
	if (memcpy_s(&m_data[0], sizeof(byte)*EachPacketSize, (*iter)->containBytes(), sizeof(byte)*EachPacketSize))
	{
		assert(false);
	}
	m_Wpos = EachPacketSize;
	while (++iter != sets.end())
	{
		m_data.resize(m_Wpos + EachPacketSize - DATA_BEGIN);
		int errorr = memcpy_s(&m_data[0] + m_Wpos, sizeof(byte)*(EachPacketSize - DATA_BEGIN), (*iter)->containBytes() + DATA_BEGIN, sizeof(byte)*(EachPacketSize - DATA_BEGIN));
		if (errorr)
		{
			assert(false);
		}
		m_Wpos += (EachPacketSize - DATA_BEGIN);
	}
}
NAMESPACE_END_CHE