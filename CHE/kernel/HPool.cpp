#include "HPool.h"
#include <algorithm>
using std::make_pair;

CHE_NAMESPACE_BEGIN

HPool::HPool()
:memory_pool(new memory_pool_type)
,memory_lock(new mutex())
,free_table(new free_table_type)
{
}

HPool::~HPool()
{
}

void * HPool::memory_malloc(size_t blocksize)
{
	static memory_pool_type::iterator iter;
	__locker(*memory_lock);
	iter = memory_pool->find(blocksize);
	if (iter != memory_pool->end()) {
		return find_first_free_memory(iter->second);
	}
	return create_new_blocksize_memory_tomap(blocksize);
}

void HPool::memory_free(void *_ptr)
{
	__locker(*memory_lock);
	static free_table_type::iterator iter;
	iter = free_table->find(_ptr);
	if (iter != free_table->end()) {
		iter->second->used = 0;
	}
	else {
		fprintf_s(stderr, "内存地址:%p释放出错！", _ptr);
	}
}

void * HPool::find_first_free_memory(pool_map_value_type & memory_set)
{
	MemoryNode *node = nullptr;
	for each(auto &val in memory_set) {
		if (!val->used) {
			node = val;
			break;
		}
	}
	if (node = nullptr) {
		node = create_new_memory_for_nofree(memory_set, memory_set.at(0)->block_size);
	}
	node->used = true;
	return node->memory_block;
}

void* HPool::create_new_blocksize_memory_tomap(size_t blocksize)
{
	MemoryNode *node = create_new_memory(blocksize);
	node->used = true;
	pool_map_value_type _v;
	_v.reserve(20);
	_v.push_back(node);
	memory_pool->insert(make_pair(blocksize, move_quick(_v)));
	return node->memory_block;
}

MemoryNode * HPool::create_new_memory_for_nofree(pool_map_value_type & memory_set, size_t blocksize)
{
	MemoryNode *node = create_new_memory(blocksize);
	memory_set.push_back(node);
	return node;
}

MemoryNode* HPool::create_new_memory(size_t blocksize)
{
	//假设所有内存申请都是成功的！
	MemoryNode *node = new MemoryNode;
	node->block_size = blocksize;
	node->memory_block = malloc(blocksize);
	free_table->insert(make_pair(node->memory_block, node));
	return node;
}


CHE_NAMESPACE_END


