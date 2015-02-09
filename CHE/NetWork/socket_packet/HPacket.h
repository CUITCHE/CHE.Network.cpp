#ifndef HPacket_h__
#define HPacket_h__

/********************************************************************
	Copyright (C) 2014 CUIT·CHE.
	All rights reserved.
	Contact: CHE (hejun_753@163.com)

	created:	2014/02/09
	created:	9:2:2014   23:01
	filename: 	e:\documents\visual studio 2013\Projects\HSystem\HSystem\HPacket.h
	file path:	e:\documents\visual studio 2013\Projects\HSystem\HSystem
	file base:	HPacket
	file ext:	h
	author:		Jq
	
	purpose:	定义 包 的虚基类，
				以后的包须继承这个类才能实例化对象
				注意：包的数据信息存储在一个结构体中，在向客户端发送数据的时候
				记得要发送完整
*********************************************************************/
#define _IOCP_TCP_MODEL_USED
#include "HDataBuffer.h"
#include "ProtocolType.h"
#include "HByteConvert.h"
#include <functional>
using std::function;
NAMESPACE_BEGIN_CHE

class HPacket
{
public:
	typedef function<void(void)> PacketWriteFunction;
	typedef function<void(void)> PacketReadFunction;

	HPacket(ProtocolType::_ProtocolType type = ProtocolType::THISISEMPTY);
	virtual ~HPacket();

	HPacket& Read(HDataBuffer& d);	//从d中实例化到Packet中。处理来自客户端来的数据
	HDataBuffer& Write();			//写到HDataBuffer中的dataBuff。完成对底层数据的操作

	H_INLINE int16 getType(){ return data.getType(); }					//获得协议类型
	H_INLINE uint32 getSize(){ return data.getDataLength(); }				//【发给客户端数据】的长度
	H_INLINE void setData(const HDataBuffer &d){data = d;}	//设置数据
	H_INLINE HDataBuffer& getData(){return data;}
	H_INLINE char* getcharData(){return (char *)data.getData();}
	H_INLINE BufferPartionSet& getBufferPartionSet(){ return data.getBufferPartionSet(); }
protected:
	//写入data模版函数
	template<typename T>
	void write_to_data(const T &d){
		HByteConvert::write(data, d);
	}
	template<typename T, typename... Args>
	void write_to_data(const T &d, Args ...args){
		HByteConvert::write(data, d);
		write_to_data(args...);
	}

	//从data读入模版函数
	template<typename T>
	void read_from_data(T &d){
		HByteConvert::read(d, data);
	}
	template<typename T, typename... Args>
	void read_from_data(T &d, Args ...args){
		HByteConvert::read(d, data);
		read_from_data(args...);
	}

	//绑定子类的写入读取函数模版
	template<typename Obj, typename Func>
	void bind(Obj *belonger, Func writer, Func reader){
		_write = std::bind(writer, belonger);
		_read = std::bind(reader, belonger);
	}
protected:
	HDataBuffer data;		//数据
	PacketWriteFunction _write;
	PacketReadFunction _read;
};
NAMESPACE_END_CHE
#endif // HPacket_h__
