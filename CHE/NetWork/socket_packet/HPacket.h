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
				�Ժ�İ���̳���������ʵ��������
				ע�⣺����������Ϣ�洢��һ���ṹ���У�����ͻ��˷������ݵ�ʱ��
				�ǵ�Ҫ��������
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

	HPacket& Read(HDataBuffer& d);	//��d��ʵ������Packet�С��������Կͻ�����������
	HDataBuffer& Write();			//д��HDataBuffer�е�dataBuff����ɶԵײ����ݵĲ���

	H_INLINE int16 getType(){ return data.getType(); }					//���Э������
	H_INLINE uint32 getSize(){ return data.getDataLength(); }				//�������ͻ������ݡ��ĳ���
	H_INLINE void setData(const HDataBuffer &d){data = d;}	//��������
	H_INLINE HDataBuffer& getData(){return data;}
	H_INLINE char* getcharData(){return (char *)data.getData();}
	H_INLINE BufferPartionSet& getBufferPartionSet(){ return data.getBufferPartionSet(); }
protected:
	//д��dataģ�溯��
	template<typename T>
	void write_to_data(const T &d){
		HByteConvert::write(data, d);
	}
	template<typename T, typename... Args>
	void write_to_data(const T &d, Args ...args){
		HByteConvert::write(data, d);
		write_to_data(args...);
	}

	//��data����ģ�溯��
	template<typename T>
	void read_from_data(T &d){
		HByteConvert::read(d, data);
	}
	template<typename T, typename... Args>
	void read_from_data(T &d, Args ...args){
		HByteConvert::read(d, data);
		read_from_data(args...);
	}

	//�������д���ȡ����ģ��
	template<typename Obj, typename Func>
	void bind(Obj *belonger, Func writer, Func reader){
		_write = std::bind(writer, belonger);
		_read = std::bind(reader, belonger);
	}
protected:
	HDataBuffer data;		//����
	PacketWriteFunction _write;
	PacketReadFunction _read;
};
NAMESPACE_END_CHE
#endif // HPacket_h__
