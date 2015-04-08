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
	** 包长(4byte)+时间戳(8byte)+data(N byte)+校验码(32byte)
	** 这里的包长，指的是除包长之外的长度。
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

	inline char* getCharData();

	getsetter(int16, type, Type)
	long long getSendTime()const;
protected:
	virtual void write(){}
	virtual void read(){}

	//写入data模版函数
	template<typename T>
	void write_to_data(const T &d){
		HByteConvert::write(*data, d);
	}
	template<typename T, typename... Args>
	void write_to_data(const T &d, const Args &...args){
		HByteConvert::write(*data, d);
		write_to_data(args...);
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
	void writeHeader();
	void readHeader();
	void setSendTime();

	//在包的尾部添加自制md5字符串32位
	void setTail();
};
CHE_NAMESPACE_END
#endif // HPacket_h__
