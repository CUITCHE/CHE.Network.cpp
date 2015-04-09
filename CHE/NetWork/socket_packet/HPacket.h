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
	** ����(4byte)+У����(32byte)+ʱ���(8byte)+data(N byte)
	** ����İ�����ָ���ǳ�����֮��ĳ��ȡ�
	   �����������Ҫ���͵�����data��������Ҫ������ӵ������ɡ����ݷ����ࡱ
	   ����
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

	inline const char* toLatin()const;

	getsetter(int16, type, Type)

protected:
	virtual void write(){}
	virtual void read(){}

	template<typename... Args>
	void write_to_data(const Args &...args){
		//���������
		int count = 0;
		__data__length(count, args...);
		setDataLength(*data, count);
		write_to_data_impl(args...);
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
	//�Ӵ����Ĳ����������ʵ�ʳ���
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

	//д��dataģ�溯��
	template<typename T>
	void write_to_data_impl(const T &d) {
		HByteConvert::write(*data, d);
	}
	//д��dataģ�溯��
	template<typename T, typename... Args>
	void write_to_data_impl(const T &d, const Args &...args) {
		HByteConvert::write(*data, d);
		write_to_data_impl(args...);
	}
};
CHE_NAMESPACE_END
#endif // HPacket_h__
