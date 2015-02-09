#ifndef HSlicePrivate_H__
#define HSlicePrivate_H__

/********************************************************************
	created:	2015/01/28
	created:	28:1:2015   14:45
	file base:	HSlicePrivate
	author:		CHE
	
	purpose:	��Ƭ��������
				
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
	typedef T _PtrType;	//ָ�������
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
	//���ø�ֵ��������
	HSlicePrivate& operator=(const HSlicePrivate& rhs) = delete;

	//��ʼ���б�ֵ��ͨ�������滻����
	HSlicePrivate<_PtrType>& operator=(initializer_list<_PtrType> ils) {
		size_t ssize = this->last - this->first;
		if (ils.size() > ssize) {
			//������ڣ�rawData��sizeҪ�����仯
			//���Ҵ���Ƭ��last���ᷢ���仯
			int newSize = this->size + ils.size() - ssize;
			_PtrType *newRawData = new _PtrType[newSize];
			//���+0�д����+1������������memcpy���ڲ�ʵ�֣��Ƿ�����0 size��copy
			memcpy(newRawData, getRawData(), sizeof(_PtrType)*(this->first + 0));
			_PtrType *pointer = newRawData;
			auto index_first = ils.begin();
			auto index_end = ils.end();
			//�滻�����µ�����
			while (index_first < index_end) {
				*(pointer++) = *(index_first++);
			}
			//����ʣ�µ�����
			for (int i = this->last; i < newSize; ++i) {
				*(pointer++) = getRawData()[i];
			}
			delete[](*rawData);
			*rawData = newRawData;
			//����last
			this->last += (ils.size() - ssize);
			this->q_ptr->size = newSize;
			this->q_ptr->last = newSize;
		}
		else {
			_PtrType *pointer = getRawData() + this->first;
			auto index_first = ils.begin();
			auto index_end = ils.end();
			//�������ı䣺�ڵ�ǰ��[first,last)֮���滻����ils; Doing
			//������Կ��Ƿ���������Ǹ�slice��ֵ
			while (index_first < index_end) {
				*(pointer++) = *(index_first++);
			}
		}
		return *this;
	}

	//ָ��һ����Ƭindex�������µ���Ƭ
	//�������Ϊ���ģ��й���Ƭȡֵ���������ڴ�
	//���������ְ���ǳ�����λstart��end��ֵ
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

	//�˺������ز���Ĭ��Ϊ1����Ƭ������
	//���п���start��end�������ԣ���һ��Ϊ�磬������һ��
	//�򷵻�nullptr
	HSlicePrivate<_PtrType>* newSliceWithoutSteps(int start, int end)
	{
		//������������[)���䣬����nullptr
		if (start == end) {
			return nullptr;
		}
		//����Ƭ�ĳ��ȣ��п��ܱ���Ƭ����ԭʼ��Ƭ��
		int ssize = this->last - this->first;
		int newEnd = end;
		//endС��0��������һ��ѭ����һ���ܣ��Ͳ�����������end��
		//�ʷ���nullptr
		if (end < 0) {
			int tmp = -end;
			//�������start > 0
			if (tmp > ssize || --tmp == start) {
				return nullptr;
			}
			//�������end����ֵ
			newEnd = ssize + end + 1;	//1-1code
		}

		//startС��0��������endС��0һ��
		//����������֮�󣬲�����1-1code���ӷ�����
		if (start < 0) {
			int tmp = -start;
			//�������end > 0
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
			//���������磺[3,2)������
		}
		else {
			//С��0���ܻ�ʹԭ�е���Ƭ���ȱ䳤���Ӷ�����һ���µ�ԭʼ��Ƭ
			//����ֻ��start<0�ſ��ܻ�ʹ��Ƭ���Ⱥ����з����仯
			//���������֧���ͻṹ��һ���µ���Ƭ����Ϊ����λ�÷����˸ı�
			int newStart = this->last + start;		//��Ƭ���
			if (end - newStart == 1) {
				//������ʼ��ֹ���غ�
				return nullptr;
			}
			if (newStart < newEnd) {
				//����������µ���Ƭ������û�з����仯���򲻲����µ�ԭʼ��Ƭ
				newSlice = new HSlicePrivate<_PtrType>(*this, this->strong_ref);
				newSlice->first = newStart;
				newSlice->last = newEnd;
			}
			else if (newStart > newEnd) {
				//�����µ���Ƭ
				int newsize = -start + newEnd;		//�õ��µ���Ƭ�ĳ���
				newSlice = new HSlicePrivate<_PtrType>(newsize);
				newSlice->rawData = new _PtrType *(new _PtrType[newsize]);
				memcpy(newSlice->getRawData(), this->getRawData() + newStart, sizeof(_PtrType)*(-start));
				memcpy(newSlice->getRawData() - start, this->getRawData(), sizeof(_PtrType)*(newEnd));
			}
		}
		return newSlice;
	}

	//�����Ƭindex��Χ�ڵ�����ָ��
	MemRef<_PtrType> getRawPointerWithIndex() {
		int num = last - first;
		if (getRawData() == 0 || num <= 0) {
			return MemRef<_PtrType>(nullptr, 0);
		}
		return MemRef<_PtrType>(getRawData() + first, num);
	}

	//�����pos�Ƿ�Ϸ�
	bool isValidOfPosition(const size_t pos)
	{
		return (int)pos >= first && (int)pos < last;
	}

	//���ù��캯��
	HSlicePrivate(const HSlicePrivate &other, int *ref)
		:rawData(other.rawData)
		,strong_ref(ref)
		,size(other.size)
		,q_ptr(other.q_ptr)
	{}

	//���캯��:������Ƭ���ܴ�С
	//Ԥ��rawData���ⲿ����
	HSlicePrivate(size_t _size)
		:first(0)
		,last(_size)
		,strong_ref(new int(1))
		,size(_size)
	{
		q_ptr = this;
	}

	//����ԭʼָ��
	_PtrType*& getRawData() { return *rawData; }

	//������������rawDataָ��
	_PtrType**& get() { return rawData; }

	_PtrType **rawData;		//ԭʼ����
	int first;				//��Ƭ��ԭʼ���ݵ���ֹλ�� ��߽磺[
	int last;				//��Ƭ��ԭʼ���ݵ���ʼλ���ұ߽磺)
	int *strong_ref;		//���ü�����ÿ����һ����Ƭ+1������һ��-1��=0����ԭʼ���ݣ�����ͬһ���ڴ棬��rawһ��ɾ��
	size_t size;			//��Ƭ�Ĵ�С��һ��ȷ�ϲ����޸ģ����ⲻ�ṩ�ӿڣ�
	HSlicePrivate<_PtrType> *q_ptr;		//ָ����ԭʼ������Private
	
};
NAMESPACE_END_CHE
#endif // HSlicePrivate_H__
