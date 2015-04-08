#include "../kernel/private/HTimerPrivate.h"
#include "htime.h"
CHE_NAMESPACE_BEGIN

atomic_uint32_t HTimerPrivate::s_timer_id = 0;
list<HTimer*> HTimerPrivate::all_timer_list;
list<HTimer*> HTimerPrivate::ready_delete_list;
mutex HTimerPrivate::all_timer_list_mtx;
mutex HTimerPrivate::ready_delete_list_mtx;

HTimerPrivate::HTimerPrivate() 
	:timer_id(++s_timer_id)
	,active(0)
	,flag_delete(0)
	,exec_once(0)
	,in_list(0)
{
}
HTimerPrivate::~HTimerPrivate()
{
	
	bool locker = all_timer_list_mtx.try_lock();
	erase_self();
	che_pointer_del(functor);
	
	if (locker) {
		all_timer_list_mtx.unlock();
	}
	
}
void HTimerPrivate::erase_self()
{
	flag_delete = 1;
}
void HTimerPrivate::append(HTimer *obj)
{
	if (!in_list) {
		in_list = 1;
		{
			__locker(all_timer_list_mtx);
			all_timer_list.push_back(obj);
			q_ptr = &all_timer_list.back();
		}
		last_exec_time = HTime::currentTime();
	}
}

bool HTimerPrivate::isValid() const
{
	return active && *q_ptr && !flag_delete && in_list;
}

CHE_NAMESPACE_END

