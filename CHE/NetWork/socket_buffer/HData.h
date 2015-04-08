#ifndef HData_H__
#define HData_H__

/********************************************************************
	created:	2015/04/08
	created:	8:4:2015   16:44
	file base:	HData
	author:		CHE
	
	purpose:	����HDataBuffer���ݳ�Ա���滻std::vector������Ч�����ڴ�
	�����ڴ��ʱ���⡣�ù̶���С��struct�γ��ڴ�������������ڴ���������
	��ϵͳ�����ǽ���**��������һ�ȡ��__data__memoryֻ�ܴ�**���л�ȡ__data__memory
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
	//�ϲ���ʹ�õ�ʱ��Ҫ��֤length��ֵ��validateMaxLength��Ӳ��ᳬ��__SIZE__
	//���������ڴ����
	void memcpy(const char *src, uint32 length)
	{
		::memcpy(data+validateMaxLength, src, sizeof(char)*length);
		validateMaxLength += length;
	}
	uint32 validateMaxLength = 0;	//���Чʹ�ó���
	__data__memory *next = nullptr;	//��һ�����ݵ�λ�ã���usedΪ0ʱ����Ϊnullptr
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
	//���ͷš�����ȥ��ʹ�õĽ��
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
	list<___Data___> *freeList;		//�����б�
	mutex *__lock__;
};
#pragma endregion

class HData
{
public:
	HData();
	~HData();
#pragma region ��ȡ����
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
