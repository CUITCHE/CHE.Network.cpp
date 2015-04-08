/********************************************************************
created:	2014/07/03
created:	3:7:2014   17:44
filename: 	e:\documents\visual studio 2013\Projects\HSystem_Qt5.3.1_2013\HSystem_Qt5.3.1_2013\net\HDataBufferPartion.h
file ext:	h
author:		CHE

purpose:	HDataBuffer的分包类
			数据包：guid【8】 + 协议【2】 + 包序号【4】 + 数据【4082】 = EachPacketSize
*********************************************************************/
#ifndef HDataBufferPartion_h__
#define HDataBufferPartion_h__
#include "HDataBufferdefs.h"
//#include <boost/pool/pool.hpp>

CHE_NAMESPACE_BEGIN

class HDataBufferPartion
{
public:
	HDataBufferPartion(const byte* head, const byte* data, size_t secParam_size);
	~HDataBufferPartion();
	inline const byte* containBytes(){ return m_dataPartBegin; }	//返回包数据的起始地址
	int64 getGUID();			//返回唯一标识符
	int16 getType();			//返回协议
	int32 getPacketTotall();	//返回包总数
	int32 getSerial();			//返回包序号
private:
	byte *m_dataPartBegin;	//数据起始地址，由boost::pool作内存管理
};
CHE_NAMESPACE_END
#endif // HDataBufferPartion_h__
