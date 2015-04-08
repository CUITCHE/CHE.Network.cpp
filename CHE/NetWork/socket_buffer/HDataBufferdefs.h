#ifndef HDataBufferdefs_H__
#define HDataBufferdefs_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014   19:27
	file base:	HDataBufferdefs
	author:		CHE
	
	purpose:	HDataBuffer的相关定义
*********************************************************************/
#include "hglobal.h"
CHE_NAMESPACE_BEGIN

#define PACKETGUID_BEGIN	0	//唯一标识码起始
#define PACKETGUID_END		7	//唯一标识码终止

#define PROTOCOL_BEGIN		8	//协议起始
#define PROTOCOL_END		9	//协议终止

#define PACKETSERIAL_BEGIN	10	//包序号起始
#define PACKETSERIAL_END	17	//包序号终止

#define DATA_BEGIN			18	//数据起始
const int EachPacketSize = 8 * 4;
#ifndef EACHPACKETSIZE
#define EACHPACKETSIZE EachPacketSize
#endif // !EACHPACKETSIZE

#define VERSION_1_0_2

#ifdef VERSION_1_0_2
/**
*	数据包类 - ver1.0.2
*	数据包：guid【8】+协议【2】+包序号【8】+数据【EachPacketSize-8-2-8】 = EachPacketSize
*	每个数据包为固定长度4kb，为了迎合iocp的缓冲区的大小而设置
*	注意发送包和接收包的区别，发送包是为了保证每个包都能发出去，定格为4kb，接收包则是所有发送包汇聚而成
*/
const int PACKETDATA_BEGIN = DATA_BEGIN;		//这个是针对于完整数据包里面的东西
const int PACKETFETCHNUMS = EachPacketSize - DATA_BEGIN;		//每次提取的数量
#endif // VERSION_1_0_2

CHE_NAMESPACE_END
#endif // HDataBufferdefs_H__
