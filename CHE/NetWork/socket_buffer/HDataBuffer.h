#ifndef HDataBuffer_h__
#define HDataBuffer_h__
/********************************************************************
	Copyright (C) 2014 CUIT·CHE.
	All rights reserved.
	Contact: CHE (hejun_753@163.com)

	created:	2014/02/08
	created:	8:2:2014   19:34
	filename: 	e:\documents\visual studio 2013\Projects\HSystem\HSystem\HDataBuffer.h
	file path:	e:\documents\visual studio 2013\Projects\HSystem\HSystem
	file base:	HDataBuffer
	file ext:	h
	author:		Jq
	
	purpose:	
	** 数据包类 - ver1.0.0（已取消这种方式的封装）
	**2015-04-08 16:22:12后的HDataBuffer类的数据格式交由外层类来控制，
	**本类只控制数据流的读写
*********************************************************************/

#include "hglobal.h"
#include <vector>
using std::vector;

CHE_NAMESPACE_BEGIN

class HDataBuffer
{
public:
	//传入协议头的长度，协议或者其它数据不归本类管辖
	HDataBuffer(uint32 writePosition);
	~HDataBuffer();
	HDataBuffer(HDataBuffer &rhs);
	HDataBuffer(HDataBuffer &&rhs);
	HDataBuffer& operator=(const HDataBuffer &d);
	void operator=(HDataBuffer &&rhs);

	inline uint32 getDataLength() const { return m_data.size(); }	//获得数据的长度【整个长度】
	inline byte* getData(){return &m_data[0];}

	//向m_nowDataBuf里写数据，并移动m_Wpos指针
	void putBytes(const byte *data, UINT len);

	//读取len长度的字节数据，返回是m_data+偏移的指针，故不再需要delete
	const byte* fetchBytes(uint32 len)const;

	//清除数据，只是把读写指针复位
	inline void clear() { m_Rpos = m_Wpos = packetBeginPosition - 1; }

private:
	uint32 packetBeginPosition;	//写入发送数据的起始位置，可能前面的位置需要腾出空间写入长度，协议等信息
	uint32 m_Wpos;				//写指针
	mutable uint32 m_Rpos;		//读指针
	vector<byte> m_data;
};
CHE_NAMESPACE_END
#endif // HDataBuffer_h__