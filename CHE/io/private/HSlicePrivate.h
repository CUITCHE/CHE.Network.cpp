#ifndef HSlicePrivate_H__
#define HSlicePrivate_H__

/********************************************************************
	created:	2015/01/28
	created:	28:1:2015   14:45
	file base:	HSlicePrivate
	author:		CHE
	
	purpose:	切片的数据类
				
*********************************************************************/
#include "HObject.h"
#include <initializer_list>
#include <iostream>
#include <ostream>

using std::cout;
using std::ostream;
using std::shared_ptr;
using std::unique_ptr;
using std::initializer_list;
NAMESPACE_BEGIN_CHE
#define INDEXPLACEHOLDER INT_MIN
struct HIndices
{
	HIndices(initializer_list<int> ils)
	{
		auto iter = ils.begin();
		first = *iter;
		if (++iter < ils.end()) {
			last = *iter;
		}
		else{
			goto ils_end;
		}
		++iter < ils.end() ? steps = *iter : che_noop();
	ils_end:
		che_noop();
	}
	int first = INDEXPLACEHOLDER;
	int last = INDEXPLACEHOLDER;
	int steps = INDEXPLACEHOLDER;
};
template<typename T> struct MemRef
{
	MemRef(T *d, size_t size)
		:data(d)
		,last(size){}
	T *data;
	int last;
	T& operator[](const size_t pos) {
		assert((int)pos < last && (int)pos >= 0);
		return (data[pos]);
	}
};
template <typename T> class HSlice;
template<typename T>
CHE_PRIVATE_DEFINED(HSlice)
{
	typedef T _PtrType;	//指针的类型
	HSlicePrivate(const T *raw, size_t _size)
		:rawData(new _PtrType*(new _PtrType[_size]))
		,size(_size)
		,first(0)
		,last(_size)
		,strong_ref(new int(1))
	{
		memcpy(getRawData(), raw, sizeof(_PtrType)*size);
		q_ptr = this;
	}
	~HSlicePrivate()
	{
		--*strong_ref;
		if (*strong_ref == 0) {
			che_pointer_del(strong_ref);
			delete[](*rawData);
			delete[]rawData;
		}
		
	}
	//禁用赋值操作函数
	HSlicePrivate& operator=(const HSlicePrivate& rhs) = delete;

	//初始化列表赋值，通常用作替换插入
	HSlicePrivate<_PtrType>& operator=(initializer_list<_PtrType> ils) {
		size_t ssize = this->last - this->first;
		if (ils.size() > ssize) {
			//如果大于，rawData的size要发生变化
			//并且此切片的last将会发生变化
			int newSize = this->size + ils.size() - ssize;
			_PtrType *newRawData = new _PtrType[newSize];
			//这儿+0有待变成+1，看编译器对memcpy的内部实现，是否允许0 size的copy
			memcpy(newRawData, getRawData(), sizeof(_PtrType)*(this->first + 0));
			_PtrType *pointer = newRawData;
			auto index_first = ils.begin();
			auto index_end = ils.end();
			//替换插入新的数据
			while (index_first < index_end) {
				*(pointer++) = *(index_first++);
			}
			//拷贝剩下的数据
			for (int i = this->last; i < newSize; ++i) {
				*(pointer++) = getRawData()[i];
			}
			delete[](*rawData);
			*rawData = newRawData;
			//更新last
			this->last += (ils.size() - ssize);
			this->q_ptr->size = newSize;
			this->q_ptr->last = newSize;
		}
		else {
			_PtrType *pointer = getRawData() + this->first;
			auto index_first = ils.begin();
			auto index_end = ils.end();
			//这里做改变：在当前的[first,last)之间替换插入ils; Doing
			//这里可以考虑返回最初的那个slice的值
			while (index_first < index_end) {
				*(pointer++) = *(index_first++);
			}
		}
		return *this;
	}

	//指定一个切片index，返回新的切片
	//这个函数为核心，有关切片取值操作，均在此
	//这个函数的职责是初步定位start和end的值
	HSlicePrivate<_PtrType>* getNewsliceFromSliceWithIndex(const HIndices &index)
	{
		++*strong_ref;
		//
		int start = index.first;
		if (start == INDEXPLACEHOLDER) {
			start = this->first;
		}
		int _end = index.last;
		if (_end == INDEXPLACEHOLDER) {
			_end = this->last;
		}
		int steps = index.steps;
		if (steps == INDEXPLACEHOLDER) {
			steps = 1;
		}
		HSlicePrivate<_PtrType>* slice = nullptr;
		if (steps == 1) {
			slice = this->newSliceWithoutSteps(start, _end);
		}
		return slice;
	}

	//此函数返回步长默认为1的切片数据类
	//其中考虑start和end的正负性，以一周为界，若超出一周
	//则返回nullptr
	HSlicePrivate<_PtrType>* newSliceWithoutSteps(int start, int end)
	{
		//不存在这样的[)区间，返回nullptr
		if (start == end) {
			return nullptr;
		}
		//求本切片的长度（有可能本切片不是原始切片）
		int ssize = this->last - this->first;
		int newEnd = end;
		//end小于0，若超过一个循环（一个周）就不存在这样的end，
		//故返回nullptr
		if (end < 0) {
			int tmp = -end;
			//这里假设start > 0
			if (tmp > ssize || --tmp == start) {
				return nullptr;
			}
			//求得正数end索引值
			newEnd = ssize + end + 1;	//1-1code
		}

		//start小于0，处理与end小于0一样
		//但满足条件之后，不做“1-1code”加法处理
		if (start < 0) {
			int tmp = -start;
			//这里假设end > 0
			if (tmp > ssize) {
				return nullptr;
			}
		}
		
		HSlicePrivate<_PtrType> *newSlice = nullptr;
		if (start >= 0) {
			if (start < end) {
				newSlice = new HSlicePrivate<_PtrType>(*this, this->strong_ref);
				newSlice->first = start;
				newSlice->last = end;
			}
			//不存在例如：[3,2)的区间
		}
		else {
			//小于0可能会使原有的切片长度变长，从而构成一个新的原始切片
			//并且只有start<0才可能会使切片长度和序列发生变化
			//进入这个分支，就会构成一个新的切片，因为序列位置发生了改变
			int newStart = this->last + start;		//切片起点
			if (end - newStart == 1) {
				//这里起始终止点重合
				return nullptr;
			}
			if (newStart < newEnd) {
				//这种情况，新的切片的序列没有发生变化，则不产生新的原始切片
				newSlice = new HSlicePrivate<_PtrType>(*this, this->strong_ref);
				newSlice->first = newStart;
				newSlice->last = newEnd;
			}
			else if (newStart > newEnd) {
				//产生新的切片
				int newsize = -start + newEnd;		//得到新的切片的长度
				newSlice = new HSlicePrivate<_PtrType>(newsize);
				newSlice->rawData = new _PtrType *(new _PtrType[newsize]);
				memcpy(newSlice->getRawData(), this->getRawData() + newStart, sizeof(_PtrType)*(-start));
				memcpy(newSlice->getRawData() - start, this->getRawData(), sizeof(_PtrType)*(newEnd));
			}
		}
		return newSlice;
	}

	//获得切片index范围内的数据指针
	MemRef<_PtrType> getRawPointerWithIndex() {
		int num = last - first;
		if (getRawData() == 0 || num <= 0) {
			return MemRef<_PtrType>(nullptr, 0);
		}
		return MemRef<_PtrType>(getRawData() + first, num);
	}

	//传入的pos是否合法
	bool isValidOfPosition(const size_t pos)
	{
		return (int)pos >= first && (int)pos < last;
	}

	//引用构造函数
	HSlicePrivate(const HSlicePrivate &other, int *ref)
		:rawData(other.rawData)
		,strong_ref(ref)
		,size(other.size)
		,q_ptr(other.q_ptr)
	{}

	//构造函数:传入切片的总大小
	//预留rawData在外部操作
	HSlicePrivate(size_t _size)
		:first(0)
		,last(_size)
		,strong_ref(new int(1))
		,size(_size)
	{
		q_ptr = this;
	}

	//返回原始指针
	_PtrType*& getRawData() { return *rawData; }

	//返回数据类中rawData指针
	_PtrType**& get() { return rawData; }

	_PtrType **rawData;		//原始数据
	int first;				//切片在原始数据的终止位置 左边界：[
	int last;				//切片在原始数据的起始位置右边界：)
	int *strong_ref;		//引用计数，每增加一个切片+1，析构一个-1，=0销毁原始数据，共享同一块内存，随raw一起删除
	size_t size;			//切片的大小，一经确认不能修改（对外不提供接口）
	HSlicePrivate<_PtrType> *q_ptr;		//指向最原始的数据Private
	
};
NAMESPACE_END_CHE
#endif // HSlicePrivate_H__
