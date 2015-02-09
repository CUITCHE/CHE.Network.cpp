#include "HObjectPrivate.h"
#include "HObject.h"

NAMESPACE_BEGIN_CHE

HObjectPrivate::HObjectPrivate()
{
	sizeof(std::mutex);
}

HObjectPrivate::~HObjectPrivate()
{
	//delete self
	if (!del_mutex) {
		return;
	}
	__locker(*del_mutex);
	children_list->remove(q_ptr);
}

void HObjectPrivate::delete_soon()
{
	del_mutex = nullptr;
}

bool HObjectPrivate::equal(const HObjectPrivate &rhs) const
{
	return parent == rhs.parent && q_ptr == rhs.q_ptr;
}

void HObjectPrivate::try_delete_children()
{
	//不是父对象就返回
	if (parent) {
		return;
	}
	__locker(*del_mutex);
	for each(auto lss in *children_list) {
		lss->d_func()->delete_soon();
		che_pointer_del(lss);
	}
}

NAMESPACE_END_CHE