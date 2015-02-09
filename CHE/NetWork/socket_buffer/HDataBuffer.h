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
	** 包长(2byte)+分包标记(4byte)+发给客户端的数据[协议:2byte + 数据:532byte]+时间戳(8byte)
	** -------0-1--------2-5----------------6-------------6-7----------8-539-------540-547
*********************************************************************/

#include "hglobal.h"
#include "HDataBufferPartion.h"
#include <vector>
#include <memory>
#include <map>
using std::vector;
using std::map;
using std::pair;
using std::shared_ptr;

NAMESPACE_BEGIN_CHE

class HDataBufferPartion;
typedef vector<shared_ptr<HDataBufferPartion>> BufferPartionSet;

class HDataBuffer
{
public:
	explicit HDataBuffer(BufferPartionSet &bufSet, bool &ok);			//用于组包，将多个HDataBufferPartion组成HDataBuffer
	HDataBuffer(int16 type);
	~HDataBuffer();
	HDataBuffer& operator=(const HDataBuffer &d);
	bool buildFromByteArray(const byte *pBuf, uint32 size);		//这个函数的调用之前请先调用getPacketSizeFromByteArry and getTimeCheckFromByteArry
	H_INLINE int16 getType() const { return m_type; }					//获得协议
	H_INLINE void setType(int16 type) {m_type = type;}				//设置协议
	H_INLINE uint32 getDataLength() const { return m_data.size(); }	//获得数据的长度【整个长度】
	H_INLINE byte* getData(){return &m_data[0];}

	//写和读的顺序都是从左到右，即先写先读
	void putBytes(const byte *data, UINT len);		//向m_nowDataBuf里写数据，并移动m_Wpos指针
	const byte* fetchBytes(uint32 len);				//读取len长度的字节数据，返回是m_data+偏移的指针，故不再需要delete
	H_INLINE void clearALL(){m_data.clear();}
	void writeOtherInfo();						//将进行拆包工作

	bool getPacketSizeFromByteArry(const byte *pBuf);//从字节数据中获得包长信息，得到正确的数据后返回true，否则返回false
	bool getTimeCheckFromByteArry(const byte *pBuf);	//从字节数据中获得时间戳，得到正确的数据后返回true，否则返回false
	H_INLINE BufferPartionSet& getBufferPartionSet(){ return m_bufferSet; }
protected:
	int32 tryFetchPACKETFETCHNUMSBytes(byte* &data);		//尝试从m_data中提取PACKETFETCHNUMS个字节数据，返回实际提取的字节数据的个数,传入的data不需要有值
	void readFromBufferSet(BufferPartionSet &sets);	//从数据集合中读取数据拷贝到本类数据成员中去
private:
	int16 m_type;				//协议类型
	UINT m_Wpos;				//写指针
	UINT m_Rpos;				//读指针
	UINT m_Fpos;				//提取指针，在每次要提取前被初始化为PACKETDATA_BEGIN
	int64 m_timeCode;			//时间戳
	int64 m_guid;				//唯一标识符
	uint32 m_packetSize;		//整个数据包的长度
	vector<byte> m_data;
	byte guidBytesInfo[PACKETDATA_BEGIN];		//数据包的前头信息
	BufferPartionSet m_bufferSet;		//所有的分包在这里聚集
};
NAMESPACE_END_CHE
#endif // HDataBuffer_h__