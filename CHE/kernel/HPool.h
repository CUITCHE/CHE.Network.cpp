#ifndef HPool_H__
#define HPool_H__

/********************************************************************
	created:	2015/04/15
	created:	15:4:2015   0:09
	file base:	HPool
	author:		CHE
	
	purpose:	内存池，专为HObject类及其子类设计
				内存池申请的内存不会主动返回给系统，它是全局存在的！
				但是你可以手动来释放它申请的内存，但是不到万不得已，
				请不要这么做，因为它的内存越多效率越快。
*********************************************************************/
#include "hglobal.h"
#include <map>
#include <vector>
#include <mutex>
using std::mutex;
using std::map;
using std::vector;


CHE_NAMESPACE_BEGIN
//最大能分配的大小，当然这是不可能这么大的
const uint32 MAX_BLOCK_SIZE = ((1 << 15) - 1);

//内存结点设计
struct MemoryNode
{
	MemoryNode():used(0),count(0),block_size(0){}
	uint32 used : 1;		//1：正在使用，0：没有被使用
	uint32 count : 16;		//使用的次数
	uint32 block_size : 15;	//分配的大小
	void *memory_block = nullptr;		//这块内存的首地址
};

class HPool
{
	typedef vector<MemoryNode*> pool_map_value_type;
	typedef map<long, pool_map_value_type> memory_pool_type;
	typedef map<void *, MemoryNode *> free_table_type;
public:
	HPool();
	~HPool();
	//向内存池申请blocksize大小的内存，返回内存的首地址，
	//失败返回nullptr
	void* memory_malloc(size_t blocksize);

	void memory_free(void *_ptr);
protected:
	//内存map中没有这样大小的内存，创造一个这样的map映射
	void* create_new_blocksize_memory_tomap(size_t blocksize);

	//满足blocksize，但没有空闲的情况
	MemoryNode* create_new_memory_for_nofree(pool_map_value_type &memory_set, size_t blocksize);

	//找到第一个空闲的内存块
	void* find_first_free_memory(pool_map_value_type &memory_set);

	//根据blocksize创建一块内存
	MemoryNode* create_new_memory(size_t blocksize);

private:
	memory_pool_type *memory_pool;	//key：分配的内存的大小，value：MemoryNode
	mutex *memory_lock;				//内存锁
	free_table_type *free_table;		//便于free
};
CHE_NAMESPACE_END

#endif // HPool_H__
