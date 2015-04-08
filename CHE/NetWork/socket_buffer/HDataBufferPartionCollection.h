#ifndef HDataBufferPartionCollection_H__
#define HDataBufferPartionCollection_H__

/********************************************************************
	created:	2014/10/10
	created:	10:10:2014   16:57
	file ext:	h
	author:		CHE
	
	purpose:	
*********************************************************************/
#include "HDataBufferPartion.h"
#include <map>
#include <memory>

using std::map;
using std::shared_ptr;

CHE_NAMESPACE_BEGIN
class HDataBufferPartion;

class HDataBufferPartionCollection
{
	typedef map<int, shared_ptr<HDataBufferPartion>> Data_Map_Type;
public:
	//�������ݰ����ϵĴ�С��һ���趨�Ͳ��ɸ���
	HDataBufferPartionCollection(size_t size);
	//����Ĭ�Ϲ��캯��
	HDataBufferPartionCollection() = delete;
	//���ø��ƹ��캯��
	HDataBufferPartionCollection(const HDataBufferPartion &) = delete;
	~HDataBufferPartionCollection();
	//�����ݰ�������������ݣ���������С����false�����ʧ�ܣ����򷵻�true�ɹ�
	//�����ݰ��������Ѻ��и����������£�
	bool append(int order, shared_ptr<HDataBufferPartion> append_data);
private:
	Data_Map_Type data_map;
	const size_t size;
};
CHE_NAMESPACE_END
#endif // HDataBufferPartionCollection_H__