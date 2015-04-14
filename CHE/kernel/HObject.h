#ifndef HObject_h__
#define HObject_h__

/********************************************************************
Copyright (C) 2014 CUIT��CHE.
All rights reserved.
Contact: CHE (hejun_753@163.com)

created:	2014/02/08
created:	8:2:2014   19:31
purpose:	������Ļ���,������ʼ���չ���������ȥ�����������
			ʵ�����ڴ����,�����౻�ͷţ�����Ҳ������ͷš�
			�ڴ�أ�
			HObject��������operator new��operator delete����ά����
			������������������ڴ棬���Լ�����һ���ڴ�أ�ʹЧ��
			��󻯡��������ص���new��Խ�࣬�ڴ�����Խ��ԽѸ�٣�
			������Ծ���Ҫnew���ֽ�����
			���䣺
			����Ϊ������Ļ��ࡣ��ֻҪ��reflection_declare�궨���ģʽ
			�̳�����ӵ�з��书�ܣ�reflection_declare�����������ԣ�
			reflection_synthesize���ںϳɷ��书�ܵ���Ҫ�������ܣ�����
			���������ļ�����Ҫ��rsc�������ɶ����cpp�ļ��������뿴��
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

	/*������ǰ�̣߳�ֱ��������ϣ���HCoreApplication������quit��̬����*/
	virtual int exec();

	/*���ر�ʾ��ǰ������ַ���*/
	virtual string to_string();

	/*ȷ��ָ���Ķ����Ƿ���ڵ�ǰ����
	��ȷ���true
	*/
	virtual bool equal(const HObject &obj)const;

	/*����ϵͳĬ�ϵĹ�ϣ���������ع�ϣֵ*/
	virtual size_t hash_code()const;

#pragma region ����operator new �� operator delete
	void* operator new(size_t blockszie);
	void operator delete(void *p);
#pragma endregion

#pragma region ������Ҫ
public:
	//��ȡ�����Class
	static Class getClass();

	//��ȡ�����Class
	virtual Class superClass();

	//���this��class_object�����࣬����true
	virtual bool isKindOfClass(Class class_object);

	//�����������
	static const char* cpp_getClassName();

	//�������Ե�ֵ
	template<typename T>
	void setValueForKey(const char *propertyname, const T &val);

	//��ȡ���Ե�ֵ
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