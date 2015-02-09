#ifndef HighEffectIOBuffer_H__
#define HighEffectIOBuffer_H__

/********************************************************************
	created:	2014/12/01
	created:	1:12:2014   15:51
	file base:	HighEffectIOBuffer
	author:		CHE
	
	purpose:	高效IO缓冲区，网络收到的字节序列交由本类管理
				本类专为多线程使用缓冲区而设计
*********************************************************************/
#include "NetIOBuffer.h"
#include <list>
#include <mutex>
#include <memory>

using std::list;
using std::mutex;
using std::unique_ptr;

NAMESPACE_BEGIN_CHE

class NetIOBuffer;

class HighEffectIOBuffer
{
	typedef list<NetIOBuffer> UseBufferType;
	struct HighEffectIOBufferData
	{
		HighEffectIOBufferData()
			:write_for_unused(new mutex)
			, read_for_used(new mutex)
			, used(new UseBufferType)
			, unused(new UseBufferType(10)) {}
		~HighEffectIOBufferData() {}
		unique_ptr<UseBufferType> used;		//已被填充数据，正等待使用
		unique_ptr<UseBufferType> unused;	//未被填充数据,初始化10个
		unique_ptr<mutex> write_for_unused;
		unique_ptr<mutex> read_for_used;
	};
public:
	HighEffectIOBuffer();
	~HighEffectIOBuffer();

	//如果有数据到来，返回true
	//否则返回false
	bool nextpadding()const;

	int unused_size()const;

	//获取一个被填充数据的buffer
	//使用本函数前调用 bool nextpadding()确保本函数成功被调用
	void buffer_used(NetIOBuffer *buffer);

	//把使用过的buffer返回缓冲池的unused
	void buffer_used(NetIOBuffer &&buffer);

	//获取一个未被填充数据的buffer
	//此类不会返回nullptr值，若返回，则表示你的代码逻辑错误！
	void buffer_unused(NetIOBuffer *buffer);

	//把填充完数据的buffer返回缓冲池的used
	void buffer_unused(NetIOBuffer &&buffer);
private:
	unique_ptr<HighEffectIOBufferData> d_ptr;
};
NAMESPACE_END_CHE
#endif // HighEffectIOBuffer_H__
