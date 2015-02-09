#include "HTimer.h"
#include "htime.h"
#include <algorithm>
NAMESPACE_BEGIN_CHE
thread* HTimer::work = nullptr;

HTimer::~HTimer()
{
	stop();
	this->killSelf();
	try_delete_invalid_timer();
}

void HTimer::start(uint32 msec /*= 5*/)
{
	Q_D(HTimer);
	if (!d->active) {
		d->active = 1;
	}
	d->interval = msec;
	
	if (!d->in_list) {
		d->append(this);
	}
}

void HTimer::stop()
{
	Q_D(HTimer);
	d->active = 0;
}

uint32 HTimer::getInterval() const
{
	Q_DC(HTimer);
	return d->interval;
}

bool HTimer::isActive() const
{
	Q_DC(HTimer);
	return d->active == 1;
}

uint32 HTimer::getTimerId() const
{
	Q_DC(HTimer);
	return d->s_timer_id;
}

bool HTimer::killTimer(uint32 timer_id)
{
	auto &functor_list = HTimerPrivate::all_timer_list;
	__locker(HTimerPrivate::all_timer_list_mtx);
	auto iter = std::find_if(functor_list.begin(), functor_list.end(), [=](const HTimer* rhs) {return rhs->getTimerId() == timer_id; });

	if (iter == functor_list.end()) {
		return false;
	}
	(*iter)->killSelf();
	return true;
}

HTimer::HTimer(HObject *parent)
	:HObject(parent)
	,d_ptr(new HTimerPrivate)
{
	_memory_dynamic_crt(work, &HTimer::execute);
}

void HTimer::singleShot_impl(TimingTask_Function_Type &func, uint32 interval)
{
	//等把CoreApplication类写完后，parent就是CoreApplication的全局对象
	HTimer *timer = new HTimer();
	timer->d_ptr->functor = new TimingTask_Function_Type(func);
	timer->d_ptr->exec_once = 1;
	timer->start(interval);
}

void HTimer::execute()
{
	auto &functor_list = HTimerPrivate::all_timer_list;
	time_t now_time = 0;
	using time_clock_callback = time_t(*)();
	time_clock_callback time_clock = HTime::currentTime;
	HTimerPrivate *d = nullptr;
	list<HTimer*>::iterator iter, iter_e;

	forever{
		if (functor_list.size() == 0) {
			__surrenderconsole__;
			continue;
		}
		{
			__locker(HTimerPrivate::all_timer_list_mtx);
			iter = functor_list.begin();
			iter_e = functor_list.end();
			while(iter != iter_e) {
				d = (*iter)->d_func();
				if (!d->isValid()) {
					++iter;
					continue;
				}
				now_time = time_clock();
				if (now_time - d->last_exec_time >= d->interval) {
					(*d->functor)();
					d->last_exec_time = now_time;
					if (d->exec_once) {
						(*iter)->killSelf();
						(*iter) = nullptr;
					}
				}
				++iter;
			}
			//删除nullptr的item
			while (std::remove(functor_list.begin(), functor_list.end(), nullptr) != functor_list.end()) {
				continue;
			}
		}
		try_delete_invalid_timer();
		__surrenderconsole__;
	}
}

void HTimer::killSelf()
{
	Q_D(HTimer);
	static list<HTimer*> &readyToDelete = HTimerPrivate::ready_delete_list;
	stop();
	d->erase_self();
	__locker(HTimerPrivate::ready_delete_list_mtx);
	readyToDelete.push_back(this);
}

void HTimer::try_delete_invalid_timer()
{
	static list<HTimer*> &readyToDelete = HTimerPrivate::ready_delete_list;
	//这个函数会被经常调用，故需要用到的变量采用static
	static list<HTimer*>::iterator iter;
	static list<HTimer*>::iterator iter_e;

	__locker(HTimerPrivate::ready_delete_list_mtx);
	iter = readyToDelete.begin();
	iter_e = readyToDelete.end();
	while (iter != iter_e) {
		delete (*iter++);
	}
	//清空待删除列表
	readyToDelete.clear();
}

NAMESPACE_END_CHE