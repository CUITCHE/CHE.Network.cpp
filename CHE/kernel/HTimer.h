#ifndef HTimer_H__
#define HTimer_H__

/********************************************************************
	Copyright (C) 2014 CUIT��CHE.
	All rights reserved.
	Contact: 	CHE (hejun_753@163.com)
	created:	2014/10/25 17:43  
	author:		CHE
	
	purpose:	��ʱ����
*********************************************************************/
#include "../kernel/private/HTimerPrivate.h"
#include <thread>
using std::thread;

CHE_NAMESPACE_BEGIN

class HTimer :public HObject
{
	CHE_DISABLE_COPY(HTimer);
	CHE_PRIVATE_DECLARE(HTimer)
public:
	template<typename _Fn>
	HTimer(_Fn fn, HObject *handle) 
		:HTimer(handle)
	{
		d_ptr->functor = new TimingTask_Function_Type(std::bind(fn, handle));
	}
	~HTimer() override;
	//��ʼһ��Timer������iʱ����ִ��
	//���timer��״̬Ϊactive��������getInterval
	void start(uint32 msec = 5);

	//�ر�һ��Timer
	void stop();

	//���ؼ��ʱ��
	uint32 getInterval()const;

	//�Ƿ������ж�����
	bool isActive()const;

	//����timer id
	uint32 getTimerId()const;

	//���һ��ִֻ��һ�ε�timer
	template<typename Func>
	static void singleShot(typename ObjectdefsPrivate::FunctionPointer<Func>::Object *handle, Func func, uint32 interval) {
		singleShot_impl(std::bind(func, handle), interval);
	}

	//����һ��timer���ɹ�����true��������������timer����false
	static bool killTimer(uint32 timer_id);
protected:
	HTimer(HObject *parent = nullptr);

	static void singleShot_impl(TimingTask_Function_Type &func, uint32 interval);

	//���ڿ������е�timer
	static void execute();

	//�Լ�ɱ���Լ��������ٱ����ã�����timerɾ��list��
	//֮���ٵ��øö��󣬿��ܻ���������Ԥ֪�ĺ��
	void killSelf();
private:
	//�ڱ�������������execute�����б����ã�����ɾ����Ч��timer
	static void try_delete_invalid_timer();
	unique_ptr<HTimerPrivate> d_ptr;
	static thread *work;
};
CHE_NAMESPACE_END
#endif // HTimer_H__
