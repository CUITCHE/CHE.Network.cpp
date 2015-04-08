#ifndef HTimer_H__
#define HTimer_H__

/********************************************************************
	Copyright (C) 2014 CUIT·CHE.
	All rights reserved.
	Contact: 	CHE (hejun_753@163.com)
	created:	2014/10/25 17:43  
	author:		CHE
	
	purpose:	定时器类
*********************************************************************/
#include "../kernel/private/HTimerPrivate.h"
#include <thread>
using std::thread;

CHE_NAMESPACE_BEGIN

class HTimer :public HObject
{
	CHE_DISABLE_COPY(HTimer);
	CHE_PRIVATE_DECLARE(HTimer)
public:
	template<typename _Fn>
	HTimer(_Fn fn, HObject *handle) 
		:HTimer(handle)
	{
		d_ptr->functor = new TimingTask_Function_Type(std::bind(fn, handle));
	}
	~HTimer() override;
	//开始一个Timer，并以i时间间隔执行
	//如果timer的状态为active，则会更新getInterval
	void start(uint32 msec = 5);

	//关闭一个Timer
	void stop();

	//返回间隔时间
	uint32 getInterval()const;

	//是否在运行队列中
	bool isActive()const;

	//返回timer id
	uint32 getTimerId()const;

	//添加一个只执行一次的timer
	template<typename Func>
	static void singleShot(typename ObjectdefsPrivate::FunctionPointer<Func>::Object *handle, Func func, uint32 interval) {
		singleShot_impl(std::bind(func, handle), interval);
	}

	//结束一个timer，成功返回true，不存在这样的timer返回false
	static bool killTimer(uint32 timer_id);
protected:
	HTimer(HObject *parent = nullptr);

	static void singleShot_impl(TimingTask_Function_Type &func, uint32 interval);

	//用于控制所有的timer
	static void execute();

	//自己杀死自己，不能再被调用，加入timer删除list中
	//之后再调用该对象，可能会引发不可预知的后果
	void killSelf();
private:
	//在本类析构函数和execute函数中被调用，尝试删除无效的timer
	static void try_delete_invalid_timer();
	unique_ptr<HTimerPrivate> d_ptr;
	static thread *work;
};
CHE_NAMESPACE_END
#endif // HTimer_H__
