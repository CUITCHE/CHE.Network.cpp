#ifndef HSlice_H__
#define HSlice_H__

/********************************************************************
	created:	2015/01/28
	created:	28:1:2015   14:45
	file base:	HSlice
	author:		CHE
	
	purpose:	��Ƭ��ͨ��[start,end,steps]��������µ���Ƭ��
				��Ƭ���������͡�
				������Ƴ��Ծ����ڵ��߳������һ��������Ч�����ϣ�
				�ʲ�֧�ֶ��̼߳��㡣����֧�ֶ��̼߳��㣬�뿪����Ա������
				�����ⲿ���м�����������Χ���Ա���ʵ���������һ�в���
*********************************************************************/
#include "../io/private/HSlicePrivate.h"

NAMESPACE_BEGIN_CHE
template<typename T>
class HSlice : public HObject
{
	inline HSlicePrivate<T>* d_func() { return d_ptr.get(); }
	inline const HSlicePrivate<T>* d_func()const { return d_ptr.get(); }
public:
	//size��T����������rawָ���еĸ���
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

	//�������ݵײ�������ָ�빲��������ֱ�ӽ���ǳ����
	HSlice(const HSlice<T> &other)
		:d_ptr(other.d_ptr)
	{
	}

	//��ʼ���б�ֵ
	HSlice<T>& operator=(initializer_list<T> ils)
	{
		auto p = d_func();
		if (p == nullptr) {
			return *this;
		}
		(*p) = ils;
		return *this;
	}

	//ͨ��operator[{start,end,steps}]����������Ƭ
	HSlice<T> operator[](const HIndices &indiceds)
	{
		HSlicePrivate<T> *tmp = d_ptr->getNewsliceFromSliceWithIndex(indiceds);
		return HSlice<T>(tmp);
	}

	//�����±�ȡֵ������Ϊ���ò���
	T& operator[](const size_t pos)
	{
		auto d = d_func();
		assert(d->isValidOfPosition(pos) == true);
		return d->getRawData()[pos];
	}

	//����val1,val2,val3�ĸ�ʽ������л���
	//����std::cout���
	//�����ݵײ�Ϊ�գ����nil
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
	//Ĭ�Ϲ��캯��
	HSlice(HSlicePrivate<T> *p)
		:d_ptr(p)
	{}
private:
	shared_ptr<HSlicePrivate<T>> d_ptr;
};
NAMESPACE_END_CHE
#endif // HSlice_H__
