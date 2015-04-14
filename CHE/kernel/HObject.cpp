#include "HObject.h"
#include "HObjectPrivate.h"
#include "HPool.h"
#include <algorithm>

CHE_NAMESPACE_BEGIN
static HPool *obj_pool = new HPool();
static Reflection<HObject> *s_HObject_reflection_obj = nullptr;

HObject::HObject(HObject *parent /*= NULL*/)
	:d_ptr(new HObjectPrivate)
	, reflection_obj(&s_HObject_reflection_obj)
{
	Q_D(HObject);
	d->q_ptr = this;
	if (!parent) {
		//������
		d->children_list = new list<HObject*>();
		d->del_mutex = new mutex;
	}
	else {
		d->parent = parent;
		auto dp = parent->d_func();
		d->children_list = dp->children_list;
		d->del_mutex = dp->del_mutex;
		__locker(*d->del_mutex);
		d->children_list->push_back(this);
	}
	DWORD funcAddr;
	__asm {
		mov eax, offset HObject::add_properties;
		mov funcAddr, eax;
	}
	initializeReflection(this, funcAddr);
}

HObject::~HObject()
{
	Q_D(HObject);
	//����ֻ��Ը������������Ӷ���ŵ�HObjectPrivate��ȥ����
	d->try_delete_children();
}

string HObject::to_string()
{
	//return string(typeid(*this).name());
	return this->cpp_getClassName();
}

bool HObject::equal(const HObject &obj) const
{
	Q_DC(HObject);
	return d->equal(*obj.d_func());
}

size_t HObject::hash_code()const
{
	return typeid(*this).hash_code();
}

int HObject::exec()
{
	return 0;
}

Class HObject::getClass()
{
	return s_HObject_reflection_obj->id;
}

Class HObject::superClass()
{
	return (*reflection_obj) ? (*reflection_obj)->id->super_class : nullptr;
}

bool HObject::isKindOfClass(Class class_object)
{
	return (*reflection_obj)->isSubOfClass(class_object);
}

const char * HObject::cpp_getClassName()
{
	return
#ifdef NO_RSC
		"HObject";
#else
		//���ʹ��RSCԪ���������ÿ������ǰ����
		//__CR������CHE Reflection
		"__CRHObject";
#endif
}
void HObject::add_properties()
{
}

void * HObject::get_class()
{
	return new HObject;
}

void* HObject::operator new(size_t blockszie)
{
	return obj_pool->memory_malloc(blockszie);
}

void HObject::operator delete(void *p)
{
	if (p) {
		obj_pool->memory_free(p);
	}
}

CHE_NAMESPACE_END
