#ifndef HTimerPrivate_H__
#define HTimerPrivate_H__

/********************************************************************
	created:	2014/12/17
	created:	17:12:2014   20:21
	file base:	HTimerPrivate
	author:		CHE
	
	purpose:	timer类的私有数据结构封装
*********************************************************************/
#include "HObject.h"
#include <list>
#include <mutex>
#include <atomic>

using std::mutex;
using std::list;
using std::atomic_uint32_t;
CHE_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
//
//define timer function
//
typedef std::function<void(void)> TimingTask_Function_Type;

class HTimer;

//无效timer id
#define INVALID_TIMER_ID (uint32)0

CHE_PRIVATE_DEFINED(HTimer)
{
	friend class HTimer;
	HTimerPrivate();
	~HTimerPrivate();
	uint32 active : 1;						//是否存活，除非被调用stop，否则将会是active，非active的timer不会被执行也不会被删除
	uint32 flag_delete : 1;					//标记删除，默认为0，即不删除，一旦为1则会被加入删除列表。
	uint32 exec_once : 1;					//执行一次
	uint32 in_list : 1;						//已被添加进all_timer_list中

	uint32 timer_id = 0;					//
	uint32 interval = 0;					//执行间隔
	__not_delete HTimer **q_ptr = nullptr;	//指向链表中HTimer对象指针的地址
	TimingTask_Function_Type *functor;		//函数体
	time_t last_exec_time = 0;				//上次调用函数的时间

	static atomic_uint32_t s_timer_id;		//给每一个timer编号从1开始，0为无效id
	static list<HTimer*> all_timer_list;	//存放所有timer的指针
	static list<HTimer*> ready_delete_list;	//准备删除list
	static mutex all_timer_list_mtx;		//list锁
	static mutex ready_delete_list_mtx;		//删除list锁


	//从链表中删除自己
	void erase_self();

	//添加到链表中
	void append(HTimer *obj);

	//是否有效，有效返回true，否则返回false
	//根据active、q_ptr、flag_delete、in_list的值来判断
	bool isValid()const;
};
CHE_NAMESPACE_END
#endif // HTimerPrivate_H__
