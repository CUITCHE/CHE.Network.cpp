/********************************************************************
	created:	2014/07/04
	created:	4:7:2014   12:49
	filename: 	e:\documents\visual studio 2013\Projects\HSystem_Qt5.3.1_2013\HSystem_Qt5.3.1_2013\net\HByteArray.h
	file ext:	h
	author:		CHE
	
	purpose:	用于托管byte数组
				必须是经过new操作符产生的数组，不然本类的析构函数会出错
				采用这个类是为了能使用std::shared_pt来管理内存
*********************************************************************/
#ifndef HByteArray_h__
#define HByteArray_h__
#include "HObject.h"
CHE_NAMESPACE_BEGIN
class HByteArray:public HObject
{
public:
	HByteArray(byte *array = NULL, size_t size = 0);
	HByteArray(const HByteArray &other) = delete;		//禁用默认拷贝
	~HByteArray();
	void reset(byte *array, size_t size);		//重置被托管的指针，原有的指针将会被delete掉
	byte* get(){ return m_array; }				//获得原始指针
	size_t size(){ return m_size; }				//获得这个数组的大小
protected:
	void destroy();		//删除现在被托管的指针
private:
	byte *m_array;		//被托管的byte数组指针
	size_t m_size;		//被托管的byte数组的大小
};
CHE_NAMESPACE_END
#endif // HByteArray_h__
