#ifndef HPool_H__
#define HPool_H__

/********************************************************************
	created:	2015/04/15
	created:	15:4:2015   0:09
	file base:	HPool
	author:		CHE
	
	purpose:	�ڴ�أ�רΪHObject�༰���������
				�ڴ��������ڴ治���������ظ�ϵͳ������ȫ�ִ��ڵģ�
				����������ֶ����ͷ���������ڴ棬���ǲ����򲻵��ѣ�
				�벻Ҫ��ô������Ϊ�����ڴ�Խ��Ч��Խ�졣
*********************************************************************/
#include "hglobal.h"
#include <map>
#include <vector>
#include <mutex>
using std::mutex;
using std::map;
using std::vector;


CHE_NAMESPACE_BEGIN
//����ܷ���Ĵ�С����Ȼ���ǲ�������ô���
const uint32 MAX_BLOCK_SIZE = ((1 << 15) - 1);

//�ڴ������
struct MemoryNode
{
	MemoryNode():used(0),count(0),block_size(0){}
	uint32 used : 1;		//1������ʹ�ã�0��û�б�ʹ��
	uint32 count : 16;		//ʹ�õĴ���
	uint32 block_size : 15;	//����Ĵ�С
	void *memory_block = nullptr;		//����ڴ���׵�ַ
};

class HPool
{
	typedef vector<MemoryNode*> pool_map_value_type;
	typedef map<long, pool_map_value_type> memory_pool_type;
	typedef map<void *, MemoryNode *> free_table_type;
public:
	HPool();
	~HPool();
	//���ڴ������blocksize��С���ڴ棬�����ڴ���׵�ַ��
	//ʧ�ܷ���nullptr
	void* memory_malloc(size_t blocksize);

	void memory_free(void *_ptr);
protected:
	//�ڴ�map��û��������С���ڴ棬����һ��������mapӳ��
	void* create_new_blocksize_memory_tomap(size_t blocksize);

	//����blocksize����û�п��е����
	MemoryNode* create_new_memory_for_nofree(pool_map_value_type &memory_set, size_t blocksize);

	//�ҵ���һ�����е��ڴ��
	void* find_first_free_memory(pool_map_value_type &memory_set);

	//����blocksize����һ���ڴ�
	MemoryNode* create_new_memory(size_t blocksize);

private:
	memory_pool_type *memory_pool;	//key��������ڴ�Ĵ�С��value��MemoryNode
	mutex *memory_lock;				//�ڴ���
	free_table_type *free_table;		//����free
};
CHE_NAMESPACE_END

#endif // HPool_H__
