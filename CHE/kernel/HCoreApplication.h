#ifndef HCoreApplication_H__
#define HCoreApplication_H__

/********************************************************************
	Copyright (C) 2014 CUIT·CHE.
	All rights reserved.
	Contact: 	CHE (hejun_753@163.com)
	created:	2014/10/25 16:58  
	author:		CHE
	
	purpose:	
*********************************************************************/
#include "HObject.h"
NAMESPACE_BEGIN_CHE
#define hApp HCoreApplication::instance()
class HCoreApplication :public HObject
{
public:
	HCoreApplication();
	~HCoreApplication();

	//重写HObject的exec
	int exec() override;

	//向HCoreApplication实例发出信号，准备退出程序，但不一定马上退出。
	static void quit(uint32 exitCode = 0);

	//返回一个HCoreApplication实例
	static const HCoreApplication* instance(){ return self; }

	//返回程序开始运行的时间点
	static const time_t appStartRunTime(){ return startRunTime; }

	//返回uint32大小的毫秒级别的时间
	static const uint32 lifeTime();

	static HCoreApplication *self;
	static const std::thread::id theMainThread;
	static time_t startRunTime;
};
NAMESPACE_END_CHE
#endif // HCoreApplication_H__
