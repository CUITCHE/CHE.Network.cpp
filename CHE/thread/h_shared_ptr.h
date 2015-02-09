#ifndef h_shared_ptr_H__
#define h_shared_ptr_H__

/********************************************************************
	created:	2014/11/24
	created:	24:11:2014   20:48
	file base:	h_shared_ptr
	author:		CHE
	
	purpose:	解决shared_ptr在循环引用下的失败计数，但本类要消耗
				不少性能作为代价，专为iocp设计的智能指针
				只管理new出来的一维对象，不提供自定义删除器接口
*********************************************************************/
#include "hglobal.h"
#include <map>
#include <mutex>
#include <atomic>
using std::map;
using std::mutex;
// #include "HObject.h"
// #include <memory>
// using std::shared_ptr;

NAMESPACE_BEGIN_CHE

static map<void*, uint32> ref_count_map;
static mutex ref_cout_mtx;
#define REF_COUNT_ERROR ((uint32)0)
template<typename _HeapAddr>
class h_shared_ptr
{
public:
	//本类只能建立在栈上
//	ONLY_STACK(h_shared_ptr);

	//创建一个空的管理对象
	h_shared_ptr(){}

	//管理传入的指针
	h_shared_ptr(_HeapAddr *_heap)
		:_heapr(_heap){
		inc_ref();
	}

	//拷贝构造函数
	h_shared_ptr(const h_shared_ptr<_HeapAddr> &rhs) 
	:_heapr(rhs._heapr)
	,ref_count(rhs.ref_count){
		if (ref_count != nullptr) {
			++(*ref_count);
		}
	}

	//移动构造函数
	h_shared_ptr(h_shared_ptr<_HeapAddr> &&rhs) { swap(rhs); }

	//安全析构
	~h_shared_ptr() {
		if (_heapr != nullptr) {
			dec_ref();
		}
	}

	//*运算
	typename ObjectdefsPrivate::StayOrigin<_HeapAddr>::Type operator*()const {
		return *_heapr;
	}

	//->运算
	_HeapAddr* operator->()const {
		return _heapr;
	}

	//显示bool转换
	explicit operator bool()const {
		return _heapr != nullptr;
	}

	//被引用的次数
	uint32 use_count()const { return ref_count == nullptr ? REF_COUNT_ERROR : *ref_count; }

	//重置指针，接手管理传入的指针
	void reset(_HeapAddr *heap) {
		dec_ref();
		_heapr = heap;
		inc_ref();
	}

	//接受来自其他h_shared_ptr的资源，共享
	h_shared_ptr<_HeapAddr>& operator=(const h_shared_ptr<_HeapAddr> &rhs) {
		if (*this != rhs) {
			dec_ref();
			ref_count = rhs.ref_count;
			_heapr = rhs._heapr;

			if (rhs) {
				++(*ref_count);
			}
		}
		return *this;
	}

	//移动赋值操作
	void operator=(h_shared_ptr<_HeapAddr> &&rhs) {
		dec_ref();
		this->swap(rhs);
	}

	friend bool operator==(const h_shared_ptr<_HeapAddr> &lhs, const h_shared_ptr<_HeapAddr> &rhs) {
		return lhs._heapr == rhs._heapr;
	}
	friend bool operator!=(const h_shared_ptr<_HeapAddr> &lhs, const h_shared_ptr<_HeapAddr> &rhs) {
		return lhs._heapr != rhs._heapr;
	}

	void swap(h_shared_ptr<_HeapAddr> &rhs) {
		std::swap(_heapr, rhs._heapr);
		std::swap(ref_count, rhs.ref_count);
	}

	//指针转换
	template<typename _Tn, typename _Ty>
	friend static h_shared_ptr<_Tn> shared_dynamic_cast(h_shared_ptr<_Ty> &rhs);

	//获取原始指针
	_HeapAddr* get() { return _heapr; }
protected:
	//减少一次引用计数
	void dec_ref() {
		if (ref_count != nullptr){
			--(*ref_count);
			if (*ref_count == 0) {
				ref_cout_mtx.lock();
				ref_count_map.erase(_heapr);
				ref_cout_mtx.unlock();
				delete _heapr;
			}
		}
		_heapr = nullptr;
		ref_count = nullptr;
	}

	//增加一次引用技术
	void inc_ref() {
		if (_heapr == nullptr) {
			return;
		}
		ref_cout_mtx.lock();
		auto iter = ref_count_map.find((void *)_heapr);
		if (iter != ref_count_map.end()) {
			ref_count = &(iter->second);
			++(*ref_count);
		}
		else {
			auto iter1 = ref_count_map.insert(std::pair<void*, uint32>((void *)_heapr, 1));
			ref_count = &(iter1.first->second);
		}
		ref_cout_mtx.unlock();
	}
private:
	_HeapAddr *_heapr = nullptr;		//要管理的堆
	uint32 *ref_count = nullptr;		//指向map中的ref_count
};

template<typename _Tn, typename _Ty>
static h_shared_ptr<_Tn>
shared_dynamic_cast(h_shared_ptr<_Ty> &rhs)
{
	static_assert(DerivedRelationship<_Ty, _Tn>::Value == true, "_Ty is not derived by _Tn.");
	auto tn = dynamic_cast<typename ObjectdefsPrivate::StayOrigin<_Tn>::Type *>(rhs._heapr);
	return h_shared_ptr<_Tn>(tn);
}
/*
template<typename T>
class h_shared_ptr
{
	typedef T* _RawPtr;
public:
	h_shared_ptr(T *p)
		:_Ptr(p)
	{}
	template<typename _Dx>
	h_shared_ptr(T *p, _Dx Dx)
		:_Ptr(p,Dx)
	{}
	_RawPtr get() {
		return _Ptr.get();
	}
	bool unique() {
		return _Ptr.unique();
	}
	long usecount()const {
		return _Ptr.use_count();
	}
	T operator
private:
	shared_ptr<T> _Ptr;
};*/
NAMESPACE_END_CHE
#endif // h_shared_ptr_H__
