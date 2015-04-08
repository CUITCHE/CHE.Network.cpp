#ifndef HTimerPrivate_H__
#define HTimerPrivate_H__

/********************************************************************
	created:	2014/12/17
	created:	17:12:2014   20:21
	file base:	HTimerPrivate
	author:		CHE
	
	purpose:	timer���˽�����ݽṹ��װ
*********************************************************************/
#include "HObject.h"
#include <list>
#include <mutex>
#include <atomic>

using std::mutex;
using std::list;
using std::atomic_uint32_t;
CHE_NAMESPACE_BEGIN
//////////////////////////////////////////////////////////////////////////
//
//define timer function
//
typedef std::function<void(void)> TimingTask_Function_Type;

class HTimer;

//��Чtimer id
#define INVALID_TIMER_ID (uint32)0

CHE_PRIVATE_DEFINED(HTimer)
{
	friend class HTimer;
	HTimerPrivate();
	~HTimerPrivate();
	uint32 active : 1;						//�Ƿ�����Ǳ�����stop�����򽫻���active����active��timer���ᱻִ��Ҳ���ᱻɾ��
	uint32 flag_delete : 1;					//���ɾ����Ĭ��Ϊ0������ɾ����һ��Ϊ1��ᱻ����ɾ���б�
	uint32 exec_once : 1;					//ִ��һ��
	uint32 in_list : 1;						//�ѱ���ӽ�all_timer_list��

	uint32 timer_id = 0;					//
	uint32 interval = 0;					//ִ�м��
	__not_delete HTimer **q_ptr = nullptr;	//ָ��������HTimer����ָ��ĵ�ַ
	TimingTask_Function_Type *functor;		//������
	time_t last_exec_time = 0;				//�ϴε��ú�����ʱ��

	static atomic_uint32_t s_timer_id;		//��ÿһ��timer��Ŵ�1��ʼ��0Ϊ��Чid
	static list<HTimer*> all_timer_list;	//�������timer��ָ��
	static list<HTimer*> ready_delete_list;	//׼��ɾ��list
	static mutex all_timer_list_mtx;		//list��
	static mutex ready_delete_list_mtx;		//ɾ��list��


	//��������ɾ���Լ�
	void erase_self();

	//��ӵ�������
	void append(HTimer *obj);

	//�Ƿ���Ч����Ч����true�����򷵻�false
	//����active��q_ptr��flag_delete��in_list��ֵ���ж�
	bool isValid()const;
};
CHE_NAMESPACE_END
#endif // HTimerPrivate_H__
