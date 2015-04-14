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
			内存池：
			HObject类重载了operator new和operator delete，它维护了
			它及其它所有子类的内存，它自己管理一个内存池，使效率
			最大化。它最大的特点是new的越多，内存分配得越快越迅速，
			尤其针对经常要new的字节数。
			反射：
			它作为反射类的基类。类只要用reflection_declare宏定义的模式
			继承它就拥有反射功能，reflection_declare用于声明属性，
			reflection_synthesize用于合成反射功能的重要函数功能，对于
			这样的类文件，需要用rsc工具生成额外的cpp文件。具体请看：
			https://github.com/CUITCHE/reflection 
*********************************************************************/
#include "Reflection.h"
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

#pragma region 重载operator new 和 operator delete
	void* operator new(size_t blockszie);
	void operator delete(void *p);
#pragma endregion

#pragma region 反射需要
public:
	//获取自身的Class
	static Class getClass();

	//获取父类的Class
	virtual Class superClass();

	//如果this是class_object的子类，返回true
	virtual bool isKindOfClass(Class class_object);

	//返回类的名字
	static const char* cpp_getClassName();

	//设置属性的值
	template<typename T>
	void setValueForKey(const char *propertyname, const T &val);

	//获取属性的值
	template<typename T>
	const T& valueForKey(const char *propertyname);
protected:
	template<typename class_type>
	friend void initializeReflection(class_type *obj, DWORD func);

	template<typename class_type>
	inline void add_property(class_type *obj) {}

	template<typename class_type, typename PropertyFunc>
	void add_property(class_type *obj, const char * propertyName, PropertyFunc get_set);

	template<typename class_type, typename PropertyFunc, typename... Args>
	void add_property(class_type *obj, const char * propertyName, PropertyFunc get_set, Args... args);

	virtual void add_properties();

	static void* get_class();
#pragma endregion

private:
	std::unique_ptr<HObjectPrivate> d_ptr;
	Reflection<HObject> **reflection_obj;
};

#ifndef REFLECTION
#define REFLECTION (*reflection_obj)
#define REFLECTION_CLASS(obj) (*(obj->reflection_obj))
#endif // !REFLECTION

template<typename class_type>
void initializeReflection(class_type *obj, DWORD func)
{
	typedef void(*ADD)(void);
	if (REFLECTION_CLASS(obj) == nullptr) {
		REFLECTION_CLASS(obj) = new Reflection<class_type>(obj);

		ADD __add = (ADD)func;
		DWORD target = (DWORD)obj;
		__asm {
			mov ecx, target;
		}
		__add();
	}
}

template <typename T>
inline void HObject::setValueForKey(const char * propertyname, const T & val)
{
	(*reflection_obj)->setValue_forKey(this, val, propertyname);
}

template <typename T>
inline const T & HObject::valueForKey(const char * propertyname)
{
	return (*reflection_obj)->value_Forkey<T>(this, propertyname);
}

template <typename class_type, typename PropertyFunc>
inline void HObject::add_property(class_type *obj, const char * propertyName, PropertyFunc get_set)
{
	(*(obj->reflection_obj))->add_property(propertyName, get_set);
}
template <typename class_type, typename PropertyFunc, typename ...Args>
inline void HObject::add_property(class_type * obj, const char * propertyName, PropertyFunc get_set, Args ...args)
{
	add_property(obj, propertyName, get_set);
	add_property(obj, args...);
}
CHE_NAMESPACE_END
#endif // HObject_h__