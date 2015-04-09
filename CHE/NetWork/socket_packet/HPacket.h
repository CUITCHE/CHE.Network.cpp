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
	** 包长(4byte)+校验码(32byte)+时间戳(8byte)+data(N byte)
	** 这里的包长，指的是除包长之外的长度。
	   这里仅仅包含要发送的数据data，其它需要额外添加的数据由“数据发送类”
	   控制
*********************************************************************/
#include "ProtocolType.h"
#include "HByteConvert.h"

CHE_NAMESPACE_BEGIN

class HPacket
{
public:

	HPacket(const ProtocolType::_ProtocolType type = ProtocolType::THISISEMPTY);
	virtual ~HPacket();

	//从d中实例化到Packet中。处理来自客户端来的数据
	HPacket& Read(const HDataBuffer& d);

	//写到HDataBuffer中的dataBuff。完成对底层数据的操作
	HDataBuffer& Write();

	inline uint32 getSize()const;

	//设置数据
	inline void setData(const HDataBuffer &d);
	inline const HDataBuffer& getData()const;

	inline const char* toLatin()const;

	getsetter(int16, type, Type)

protected:
	virtual void write(){}
	virtual void read(){}

	template<typename... Args>
	void write_to_data(const Args &...args){
		//计算包长度
		int count = 0;
		__data__length(count, args...);
		setDataLength(*data, count);
		write_to_data_impl(args...);
	}

	//从data读入模版函数
	template<typename T>
	void read_from_data(T &d){
		HByteConvert::read(d, *data);
	}
	template<typename T, typename... Args>
	void read_from_data(T &d, Args &...args){
		HByteConvert::read(d, *data);
		read_from_data(args...);
	}
	
protected:
	int16 type;
	HDataBuffer *data = nullptr;
private:
	//从传来的参数计算包的实际长度
	template<typename T>
	void __data__length(int &count, const T &d) {
		count += sizeof(T);
	}
	template<typename T, typename... Args>
	void __data__length(int &count, const T &d, const Args &...args) {
		
		__data__length(count, d);
		__data__length(count, args...);
	}
	template<>
	void __data__length<string>(int &count, const string &d) {
		count += (sizeof(int) + d.size()*sizeof(char));

	}

	//写入data模版函数
	template<typename T>
	void write_to_data_impl(const T &d) {
		HByteConvert::write(*data, d);
	}
	//写入data模版函数
	template<typename T, typename... Args>
	void write_to_data_impl(const T &d, const Args &...args) {
		HByteConvert::write(*data, d);
		write_to_data_impl(args...);
	}
};
CHE_NAMESPACE_END
#endif // HPacket_h__
