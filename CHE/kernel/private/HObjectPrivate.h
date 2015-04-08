#ifndef HObjectPrivate_H__
#define HObjectPrivate_H__

/********************************************************************
	created:	2014/12/19
	created:	19:12:2014   12:11
	file base:	HObjectPrivate
	author:		CHE
	
	purpose:	HObject底层数据封装
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

	list<HObject*> *children_list = nullptr;	//存储以自己为parent的对象指针
	__not_delete HObject *parent = nullptr;		//父对象 若父对象是自己，则为nullptr
	__not_delete HObject *q_ptr = nullptr;		//指向OO对象
	mutex *del_mutex = nullptr;					//删除指针对象的锁

	//标记已删除
	void delete_soon();

	//判断是否相等
	bool equal(const HObjectPrivate &rhs) const;

	//删除所有子对象
	void try_delete_children();
};
CHE_NAMESPACE_END
#endif // HObjectPrivate_H__
