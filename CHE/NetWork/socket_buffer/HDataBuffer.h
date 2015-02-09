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
	** ����(2byte)+�ְ����(4byte)+�����ͻ��˵�����[Э��:2byte + ����:532byte]+ʱ���(8byte)
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
	explicit HDataBuffer(BufferPartionSet &bufSet, bool &ok);			//��������������HDataBufferPartion���HDataBuffer
	HDataBuffer(int16 type);
	~HDataBuffer();
	HDataBuffer& operator=(const HDataBuffer &d);
	bool buildFromByteArray(const byte *pBuf, uint32 size);		//��������ĵ���֮ǰ���ȵ���getPacketSizeFromByteArry and getTimeCheckFromByteArry
	H_INLINE int16 getType() const { return m_type; }					//���Э��
	H_INLINE void setType(int16 type) {m_type = type;}				//����Э��
	H_INLINE uint32 getDataLength() const { return m_data.size(); }	//������ݵĳ��ȡ��������ȡ�
	H_INLINE byte* getData(){return &m_data[0];}

	//д�Ͷ���˳���Ǵ����ң�����д�ȶ�
	void putBytes(const byte *data, UINT len);		//��m_nowDataBuf��д���ݣ����ƶ�m_Wposָ��
	const byte* fetchBytes(uint32 len);				//��ȡlen���ȵ��ֽ����ݣ�������m_data+ƫ�Ƶ�ָ�룬�ʲ�����Ҫdelete
	H_INLINE void clearALL(){m_data.clear();}
	void writeOtherInfo();						//�����в������

	bool getPacketSizeFromByteArry(const byte *pBuf);//���ֽ������л�ð�����Ϣ���õ���ȷ�����ݺ󷵻�true�����򷵻�false
	bool getTimeCheckFromByteArry(const byte *pBuf);	//���ֽ������л��ʱ������õ���ȷ�����ݺ󷵻�true�����򷵻�false
	H_INLINE BufferPartionSet& getBufferPartionSet(){ return m_bufferSet; }
protected:
	int32 tryFetchPACKETFETCHNUMSBytes(byte* &data);		//���Դ�m_data����ȡPACKETFETCHNUMS���ֽ����ݣ�����ʵ����ȡ���ֽ����ݵĸ���,�����data����Ҫ��ֵ
	void readFromBufferSet(BufferPartionSet &sets);	//�����ݼ����ж�ȡ���ݿ������������ݳ�Ա��ȥ
private:
	int16 m_type;				//Э������
	UINT m_Wpos;				//дָ��
	UINT m_Rpos;				//��ָ��
	UINT m_Fpos;				//��ȡָ�룬��ÿ��Ҫ��ȡǰ����ʼ��ΪPACKETDATA_BEGIN
	int64 m_timeCode;			//ʱ���
	int64 m_guid;				//Ψһ��ʶ��
	uint32 m_packetSize;		//�������ݰ��ĳ���
	vector<byte> m_data;
	byte guidBytesInfo[PACKETDATA_BEGIN];		//���ݰ���ǰͷ��Ϣ
	BufferPartionSet m_bufferSet;		//���еķְ�������ۼ�
};
NAMESPACE_END_CHE
#endif // HDataBuffer_h__