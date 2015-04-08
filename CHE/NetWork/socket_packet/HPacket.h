#ifndef HPacket_h__
#define HPacket_h__

/********************************************************************
	Copyright (C) 2014 CUIT��CHE.
	All rights reserved.
	Contact: CHE (hejun_753@163.com)

	created:	2014/02/09
	created:	9:2:2014   23:01
	filename: 	e:\documents\visual studio 2013\Projects\HSystem\HSystem\HPacket.h
	file path:	e:\documents\visual studio 2013\Projects\HSystem\HSystem
	file base:	HPacket
	file ext:	h
	author:		Jq
	
	purpose:	���� �� ������࣬
	** ����(4byte)+ʱ���(8byte)+data(N byte)+У����(32byte)
	** ����İ�����ָ���ǳ�����֮��ĳ��ȡ�
*********************************************************************/
#include "ProtocolType.h"
#include "HByteConvert.h"

CHE_NAMESPACE_BEGIN

class HPacket
{
public:

	HPacket(const ProtocolType::_ProtocolType type = ProtocolType::THISISEMPTY);
	virtual ~HPacket();

	//��d��ʵ������Packet�С��������Կͻ�����������
	HPacket& Read(const HDataBuffer& d);

	//д��HDataBuffer�е�dataBuff����ɶԵײ����ݵĲ���
	HDataBuffer& Write();

	inline uint32 getSize()const;

	//��������
	inline void setData(const HDataBuffer &d);
	inline const HDataBuffer& getData()const;

	inline char* getCharData();

	getsetter(int16, type, Type)
	long long getSendTime()const;
protected:
	virtual void write(){}
	virtual void read(){}

	//д��dataģ�溯��
	template<typename T>
	void write_to_data(const T &d){
		HByteConvert::write(*data, d);
	}
	template<typename T, typename... Args>
	void write_to_data(const T &d, const Args &...args){
		HByteConvert::write(*data, d);
		write_to_data(args...);
	}

	//��data����ģ�溯��
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

	//�ڰ���β���������md5�ַ���32λ
	void setTail();
};
CHE_NAMESPACE_END
#endif // HPacket_h__
