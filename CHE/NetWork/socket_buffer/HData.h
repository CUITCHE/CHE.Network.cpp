#ifndef HData_H__
#define HData_H__

/********************************************************************
	created:	2015/04/08
	created:	8:4:2015   16:44
	file base:	HData
	author:		CHE
	
	purpose:	用于HDataBuffer数据成员，替换std::vector，来高效操作内存
	减少内存耗时问题。用固定大小的struct形成内存链表，申请过的内存链表不交回
	给系统，而是交给**类管理，并且获取的__data__memory只能从**类中获取__data__memory
*********************************************************************/
#include "hglobal.h"
#include <list>
#include <mutex>
using std::list;
using std::mutex;
CHE_NAMESPACE_BEGIN

const int32 __SIZE__ = 1 << 5;
#pragma region __data__memory
struct __data__memory
{
	//上层在使用的时候，要保证length的值与validateMaxLength相加不会超过__SIZE__
	//否则会出现内存错误
	void memcpy(const char *src, uint32 length)
	{
		::memcpy(data+validateMaxLength, src, sizeof(char)*length);
		validateMaxLength += length;
	}
	uint32 validateMaxLength = 0;	//最长有效使用长度
	__data__memory *next = nullptr;	//下一条数据的位置，当used为0时，必为nullptr
	char data[__SIZE__];
};
#pragma endregion

#pragma region __memory__pool

template<typename T>
struct __memory__pool
{
	typedef T* ___Data___;

	__memory__pool()
		:__lock__(new mutex())
		, freeList(new list<___Data___>)
	{
		for (int i = 0; i < 10; ++i) {
			freeList->push_back(new T);
		}
	}
	~__memory__pool()
	{
		__lock__->lock();
		for each(auto var in *freeList) {
			delete *var;
		}
		delete freeList;
		__lock__->unlock();
		delete __lock__;
	}
	//“释放”被拿去被使用的结点
	inline void recoveryMemory(___Data___ mem)
	{
		__locker(*__lock__);
		freeList->push_back(mem);
	}
	inline ___Data___ getMemory()
	{
		__locker(*__lock__);
		if (freeList->empty() == true) {
			freeList->push_back(new T);
		}
		___Data___ tmp = freeList->front();
		freeList->pop_front();
		return tmp;
	}
private:
	list<___Data___> *freeList;		//空闲列表
	mutex *__lock__;
};
#pragma endregion

class HData
{
public:
	HData();
	~HData();
#pragma region 获取数据
	void resetNextObjectPointer();
	bool next();
	__data__memory* Object();
#pragma endregion

private:
	__data__memory *__pointer__ = nullptr;
	__data__memory *__data__header = nullptr;
	static __memory__pool<__data__memory> *__pool__;
};
CHE_NAMESPACE_END

#endif // HData_H__
