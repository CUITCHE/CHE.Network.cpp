#ifndef HObjectPrivate_H__
#define HObjectPrivate_H__

/********************************************************************
	created:	2014/12/19
	created:	19:12:2014   12:11
	file base:	HObjectPrivate
	author:		CHE
	
	purpose:	HObject�ײ����ݷ�װ
*********************************************************************/
#include "hglobal.h"
#include <list>
#include <mutex>
using std::list;
using std::mutex;
CHE_NAMESPACE_BEGIN

CHE_PRIVATE_DEFINED(HObject)
{
	friend class HObject;
	HObjectPrivate();
	~HObjectPrivate();

	list<HObject*> *children_list = nullptr;	//�洢���Լ�Ϊparent�Ķ���ָ��
	__not_delete HObject *parent = nullptr;		//������ �����������Լ�����Ϊnullptr
	__not_delete HObject *q_ptr = nullptr;		//ָ��OO����
	mutex *del_mutex = nullptr;					//ɾ��ָ��������

	//�����ɾ��
	void delete_soon();

	//�ж��Ƿ����
	bool equal(const HObjectPrivate &rhs) const;

	//ɾ�������Ӷ���
	void try_delete_children();
};
CHE_NAMESPACE_END
#endif // HObjectPrivate_H__
