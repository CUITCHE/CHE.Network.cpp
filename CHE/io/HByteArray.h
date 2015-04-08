/********************************************************************
	created:	2014/07/04
	created:	4:7:2014   12:49
	filename: 	e:\documents\visual studio 2013\Projects\HSystem_Qt5.3.1_2013\HSystem_Qt5.3.1_2013\net\HByteArray.h
	file ext:	h
	author:		CHE
	
	purpose:	�����й�byte����
				�����Ǿ���new���������������飬��Ȼ������������������
				�����������Ϊ����ʹ��std::shared_pt�������ڴ�
*********************************************************************/
#ifndef HByteArray_h__
#define HByteArray_h__
#include "HObject.h"
CHE_NAMESPACE_BEGIN
class HByteArray:public HObject
{
public:
	HByteArray(byte *array = NULL, size_t size = 0);
	HByteArray(const HByteArray &other) = delete;		//����Ĭ�Ͽ���
	~HByteArray();
	void reset(byte *array, size_t size);		//���ñ��йܵ�ָ�룬ԭ�е�ָ�뽫�ᱻdelete��
	byte* get(){ return m_array; }				//���ԭʼָ��
	size_t size(){ return m_size; }				//����������Ĵ�С
protected:
	void destroy();		//ɾ�����ڱ��йܵ�ָ��
private:
	byte *m_array;		//���йܵ�byte����ָ��
	size_t m_size;		//���йܵ�byte����Ĵ�С
};
CHE_NAMESPACE_END
#endif // HByteArray_h__
