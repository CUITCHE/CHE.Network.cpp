#include "HThread.h"
CHE_NAMESPACE_BEGIN
thread* HThread::thread_timer = nullptr;
HThread::HThread(HObject *parent /*= 0*/)
:HObject(parent)
{
}



HThread::~HThread()
{
	tryStop();
}

void HThread::tryStop()
{
	if (thread_general == nullptr) {
		return;
	}
	che_pointer_before_del(thread_general, &thread::join);
}

CHE_NAMESPACE_END
