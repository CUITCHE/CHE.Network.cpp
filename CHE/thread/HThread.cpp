#include "HThread.h"
NAMESPACE_BEGIN_CHE
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

NAMESPACE_END_CHE