#ifndef HThread_H__
#define HThread_H__

/********************************************************************
	created:	2014/11/24
	created:	24:11:2014   18:42
	file base:	HThread
	author:		CHE
	
	purpose:	�򵥷�װ��׼���thread
*********************************************************************/
#include "HObject.h"
#include <thread>
using std::thread;

CHE_NAMESPACE_BEGIN

class HThread :public HObject
{
	CHE_DISABLE_COPY(HThread)
public:
	HThread(HObject *parent = 0);
	~HThread();

	void tryStop();
	void* handle() { return thread_general->native_handle(); }
protected:
	template<typename _Fn>
	void run(_Fn fn, typename ObjectdefsPrivate::FunctionPointer<_Fn>::Object *object) {
		_memory_dynamic_crt(thread_general, fn, object);
	}
private:
	thread *thread_general = nullptr;		//��ͨ�߳�
	static thread *thread_timer;			//����timer��ĺ�������
};
CHE_NAMESPACE_END
#endif // HThread_H__
