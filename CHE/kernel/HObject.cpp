#include "HObject.h"
#include "HObjectPrivate.h"
#include <algorithm>

CHE_NAMESPACE_BEGIN


HObject::HObject(HObject *parent /*= NULL*/)
:d_ptr(new HObjectPrivate)
{
	Q_D(HObject);
	d->q_ptr = this;
	if (!parent) {
		//父对象
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
}

HObject::~HObject()
{
	Q_D(HObject);
	//这里只针对父对象作处理，子对象放到HObjectPrivate中去处理
	d->try_delete_children();
}

string HObject::to_string()
{
	return string(typeid(*this).name());
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

CHE_NAMESPACE_END
