/********************************************************************
created:	2014/07/03
created:	3:7:2014   17:44
filename: 	e:\documents\visual studio 2013\Projects\HSystem_Qt5.3.1_2013\HSystem_Qt5.3.1_2013\net\HDataBufferPartion.h
file ext:	h
author:		CHE

purpose:	HDataBuffer�ķְ���
			���ݰ���guid��8�� + Э�顾2�� + ����š�4�� + ���ݡ�4082�� = EachPacketSize
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
	inline const byte* containBytes(){ return m_dataPartBegin; }	//���ذ����ݵ���ʼ��ַ
	int64 getGUID();			//����Ψһ��ʶ��
	int16 getType();			//����Э��
	int32 getPacketTotall();	//���ذ�����
	int32 getSerial();			//���ذ����
private:
	byte *m_dataPartBegin;	//������ʼ��ַ����boost::pool���ڴ����
};
CHE_NAMESPACE_END
#endif // HDataBufferPartion_h__
