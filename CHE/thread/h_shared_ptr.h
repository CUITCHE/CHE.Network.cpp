#ifndef h_shared_ptr_H__
#define h_shared_ptr_H__

/********************************************************************
	created:	2014/11/24
	created:	24:11:2014   20:48
	file base:	h_shared_ptr
	author:		CHE
	
	purpose:	���shared_ptr��ѭ�������µ�ʧ�ܼ�����������Ҫ����
				����������Ϊ���ۣ�רΪiocp��Ƶ�����ָ��
				ֻ����new������һά���󣬲��ṩ�Զ���ɾ�����ӿ�
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
	//����ֻ�ܽ�����ջ��
//	ONLY_STACK(h_shared_ptr);

	//����һ���յĹ������
	h_shared_ptr(){}

	//�������ָ��
	h_shared_ptr(_HeapAddr *_heap)
		:_heapr(_heap){
		inc_ref();
	}

	//�������캯��
	h_shared_ptr(const h_shared_ptr<_HeapAddr> &rhs) 
	:_heapr(rhs._heapr)
	,ref_count(rhs.ref_count){
		if (ref_count != nullptr) {
			++(*ref_count);
		}
	}

	//�ƶ����캯��
	h_shared_ptr(h_shared_ptr<_HeapAddr> &&rhs) { swap(rhs); }

	//��ȫ����
	~h_shared_ptr() {
		if (_heapr != nullptr) {
			dec_ref();
		}
	}

	//*����
	typename ObjectdefsPrivate::StayOrigin<_HeapAddr>::Type operator*()const {
		return *_heapr;
	}

	//->����
	_HeapAddr* operator->()const {
		return _heapr;
	}

	//��ʾboolת��
	explicit operator bool()const {
		return _heapr != nullptr;
	}

	//�����õĴ���
	uint32 use_count()const { return ref_count == nullptr ? REF_COUNT_ERROR : *ref_count; }

	//����ָ�룬���ֹ������ָ��
	void reset(_HeapAddr *heap) {
		dec_ref();
		_heapr = heap;
		inc_ref();
	}

	//������������h_shared_ptr����Դ������
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

	//�ƶ���ֵ����
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

	//ָ��ת��
	template<typename _Tn, typename _Ty>
	friend static h_shared_ptr<_Tn> shared_dynamic_cast(h_shared_ptr<_Ty> &rhs);

	//��ȡԭʼָ��
	_HeapAddr* get() { return _heapr; }
protected:
	//����һ�����ü���
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

	//����һ�����ü���
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
	_HeapAddr *_heapr = nullptr;		//Ҫ����Ķ�
	uint32 *ref_count = nullptr;		//ָ��map�е�ref_count
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
