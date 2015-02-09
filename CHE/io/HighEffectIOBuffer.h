#ifndef HighEffectIOBuffer_H__
#define HighEffectIOBuffer_H__

/********************************************************************
	created:	2014/12/01
	created:	1:12:2014   15:51
	file base:	HighEffectIOBuffer
	author:		CHE
	
	purpose:	��ЧIO�������������յ����ֽ����н��ɱ������
				����רΪ���߳�ʹ�û����������
*********************************************************************/
#include "NetIOBuffer.h"
#include <list>
#include <mutex>
#include <memory>

using std::list;
using std::mutex;
using std::unique_ptr;

NAMESPACE_BEGIN_CHE

class NetIOBuffer;

class HighEffectIOBuffer
{
	typedef list<NetIOBuffer> UseBufferType;
	struct HighEffectIOBufferData
	{
		HighEffectIOBufferData()
			:write_for_unused(new mutex)
			, read_for_used(new mutex)
			, used(new UseBufferType)
			, unused(new UseBufferType(10)) {}
		~HighEffectIOBufferData() {}
		unique_ptr<UseBufferType> used;		//�ѱ�������ݣ����ȴ�ʹ��
		unique_ptr<UseBufferType> unused;	//δ���������,��ʼ��10��
		unique_ptr<mutex> write_for_unused;
		unique_ptr<mutex> read_for_used;
	};
public:
	HighEffectIOBuffer();
	~HighEffectIOBuffer();

	//��������ݵ���������true
	//���򷵻�false
	bool nextpadding()const;

	int unused_size()const;

	//��ȡһ����������ݵ�buffer
	//ʹ�ñ�����ǰ���� bool nextpadding()ȷ���������ɹ�������
	void buffer_used(NetIOBuffer *buffer);

	//��ʹ�ù���buffer���ػ���ص�unused
	void buffer_used(NetIOBuffer &&buffer);

	//��ȡһ��δ��������ݵ�buffer
	//���಻�᷵��nullptrֵ�������أ����ʾ��Ĵ����߼�����
	void buffer_unused(NetIOBuffer *buffer);

	//����������ݵ�buffer���ػ���ص�used
	void buffer_unused(NetIOBuffer &&buffer);
private:
	unique_ptr<HighEffectIOBufferData> d_ptr;
};
NAMESPACE_END_CHE
#endif // HighEffectIOBuffer_H__
