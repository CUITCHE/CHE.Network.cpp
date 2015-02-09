#ifndef HDataBufferPartionCollection_H__
#define HDataBufferPartionCollection_H__

/********************************************************************
	created:	2014/10/10
	created:	10:10:2014   16:57
	file ext:	h
	author:		CHE
	
	purpose:	
*********************************************************************/
#include "HDataBufferPartion.h"
#include <map>
#include <memory>

using std::map;
using std::shared_ptr;

NAMESPACE_BEGIN_CHE
class HDataBufferPartion;

class HDataBufferPartionCollection
{
	typedef map<int, shared_ptr<HDataBufferPartion>> Data_Map_Type;
public:
	//设置数据包集合的大小，一旦设定就不可更改
	HDataBufferPartionCollection(size_t size);
	//禁用默认构造函数
	HDataBufferPartionCollection() = delete;
	//禁用复制构造函数
	HDataBufferPartionCollection(const HDataBufferPartion &) = delete;
	~HDataBufferPartionCollection();
	//向数据包集合内添加数据，若超出大小返回false即添加失败，否则返回true成功
	//若数据包集合内已含有该数据则会更新，
	bool append(int order, shared_ptr<HDataBufferPartion> append_data);
private:
	Data_Map_Type data_map;
	const size_t size;
};
NAMESPACE_END_CHE
#endif // HDataBufferPartionCollection_H__