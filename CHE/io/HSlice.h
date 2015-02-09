#ifndef HSlice_H__
#define HSlice_H__

/********************************************************************
	created:	2015/01/28
	created:	28:1:2015   14:45
	file base:	HSlice
	author:		CHE
	
	purpose:	切片，通过[start,end,steps]索引获得新的切片，
				切片是引用类型。
				本类设计初衷就是在单线程里完成一切事务并且效率至上，
				故不支持多线程计算。若想支持多线程计算，请开发人员自行在
				本类外部进行加锁。加锁范围：对本类实例化对象的一切操作
*********************************************************************/
#include "../io/private/HSlicePrivate.h"

NAMESPACE_BEGIN_CHE
template<typename T>
class HSlice : public HObject
{
	inline HSlicePrivate<T>* d_func() { return d_ptr.get(); }
	inline const HSlicePrivate<T>* d_func()const { return d_ptr.get(); }
public:
	//size：T类型数据在raw指针中的个数
	HSlice(const T *raw, size_t size)
		:d_ptr(new HSlicePrivate<T>(raw,size))
	{}
	HSlice(initializer_list<T> ils)
	{
		int size = ils.size();
		HSlicePrivate<T> *dslice = new HSlicePrivate<T>(size);
		dslice->rawData = new T*(new T[size]);
		auto poniter = dslice->getRawData();
		for (auto val = ils.begin(); val < ils.end(); ++val) {
			*(poniter++) = *val;
		}
		d_ptr.reset(dslice);
	}
	~HSlice()override{}

	//由于数据底层是数据指针共享，故这里直接进行浅复制
	HSlice(const HSlice<T> &other)
		:d_ptr(other.d_ptr)
	{
	}

	//初始化列表赋值
	HSlice<T>& operator=(initializer_list<T> ils)
	{
		auto p = d_func();
		if (p == nullptr) {
			return *this;
		}
		(*p) = ils;
		return *this;
	}

	//通过operator[{start,end,steps}]产生引用切片
	HSlice<T> operator[](const HIndices &indiceds)
	{
		HSlicePrivate<T> *tmp = d_ptr->getNewsliceFromSliceWithIndex(indiceds);
		return HSlice<T>(tmp);
	}

	//数组下标取值操作，为引用操作
	T& operator[](const size_t pos)
	{
		auto d = d_func();
		assert(d->isValidOfPosition(pos) == true);
		return d->getRawData()[pos];
	}

	//按照val1,val2,val3的格式输出，有换行
	//采用std::cout输出
	//若数据底层为空，输出nil
	friend ostream& operator<<(ostream &out, HSlice<T> &slice) {
		auto d = slice.d_func();
		if (d == nullptr) {
			cout << "nil" << endl;
			return out;
		}
		auto raw = d->getRawPointerWithIndex();
		if (raw.data == nullptr) {
			cout << "nil" << endl;
			return out;
		}
		size_t length = raw.last;
		cout << raw[0];
		for (size_t i = 1; i < length; ++i) {
			putchar(',');
			cout << raw[i];
		}
		cout << endl;
		return out;
	}
private:
	//默认构造函数
	HSlice(HSlicePrivate<T> *p)
		:d_ptr(p)
	{}
private:
	shared_ptr<HSlicePrivate<T>> d_ptr;
};
NAMESPACE_END_CHE
#endif // HSlice_H__
