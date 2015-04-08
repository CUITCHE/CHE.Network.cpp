#ifndef HObject_h__
#define HObject_h__

/********************************************************************
Copyright (C) 2014 CUIT·CHE.
All rights reserved.
Contact: CHE (hejun_753@163.com)

created:	2014/02/08
created:	8:2:2014   19:31
purpose:	所有类的基类,本类自始至终管理派生出去的所有子类的
			实例的内存管理,当父类被释放，子类也会跟着释放。
*********************************************************************/
#include "hglobal.h"
#include <memory>
using std::unique_ptr;
using std::string;
CHE_NAMESPACE_BEGIN

struct HObjectPrivate;
class HObject
{
	friend struct HObjectPrivate;
	CHE_PRIVATE_DECLARE(HObject)
public:
	HObject(HObject *parent = 0);
	virtual ~HObject();

	/*阻塞当前线程，直到运行完毕，或HCoreApplication被调用quit静态方法*/
	virtual int exec();

	/*返回表示当前对象的字符串*/
	virtual string to_string();

	/*确定指定的对象是否等于当前对象
	相等返回true
	*/
	virtual bool equal(const HObject &obj)const;

	/*调用系统默认的哈希函数，返回哈希值*/
	virtual size_t hash_code()const;
private:
	std::unique_ptr<HObjectPrivate> d_ptr;
};
CHE_NAMESPACE_END
#endif // HObject_h__