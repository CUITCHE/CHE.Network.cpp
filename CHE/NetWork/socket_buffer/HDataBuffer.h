#ifndef HDataBuffer_h__
#define HDataBuffer_h__
/********************************************************************
	Copyright (C) 2014 CUIT��CHE.
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
	** ���ݰ��� - ver1.0.0����ȡ�����ַ�ʽ�ķ�װ��
	**2015-04-08 16:22:12���HDataBuffer������ݸ�ʽ��������������ƣ�
	**����ֻ�����������Ķ�д
*********************************************************************/

#include "hglobal.h"
#include <vector>
using std::vector;

CHE_NAMESPACE_BEGIN

class HDataBuffer
{
public:
	//����Э��ͷ�ĳ��ȣ�Э������������ݲ��鱾���Ͻ
	HDataBuffer(uint32 writePosition);
	~HDataBuffer();
	HDataBuffer(HDataBuffer &rhs);
	HDataBuffer(HDataBuffer &&rhs);
	HDataBuffer& operator=(const HDataBuffer &d);
	void operator=(HDataBuffer &&rhs);

	inline uint32 getDataLength() const { return m_data.size(); }	//������ݵĳ��ȡ��������ȡ�
	inline byte* getData(){return &m_data[0];}

	//��m_nowDataBuf��д���ݣ����ƶ�m_Wposָ��
	void putBytes(const byte *data, UINT len);

	//��ȡlen���ȵ��ֽ����ݣ�������m_data+ƫ�Ƶ�ָ�룬�ʲ�����Ҫdelete
	const byte* fetchBytes(uint32 len)const;

	//������ݣ�ֻ�ǰѶ�дָ�븴λ
	inline void clear() { m_Rpos = m_Wpos = packetBeginPosition - 1; }

private:
	uint32 packetBeginPosition;	//д�뷢�����ݵ���ʼλ�ã�����ǰ���λ����Ҫ�ڳ��ռ�д�볤�ȣ�Э�����Ϣ
	uint32 m_Wpos;				//дָ��
	mutable uint32 m_Rpos;		//��ָ��
	vector<byte> m_data;
};
CHE_NAMESPACE_END
#endif // HDataBuffer_h__