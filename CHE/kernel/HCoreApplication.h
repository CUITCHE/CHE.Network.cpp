#ifndef HCoreApplication_H__
#define HCoreApplication_H__

/********************************************************************
	Copyright (C) 2014 CUIT��CHE.
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

	//��дHObject��exec
	int exec() override;

	//��HCoreApplicationʵ�������źţ�׼���˳����򣬵���һ�������˳���
	static void quit(uint32 exitCode = 0);

	//����һ��HCoreApplicationʵ��
	static const HCoreApplication* instance(){ return self; }

	//���س���ʼ���е�ʱ���
	static const time_t appStartRunTime(){ return startRunTime; }

	//����uint32��С�ĺ��뼶���ʱ��
	static const uint32 lifeTime();

	static HCoreApplication *self;
	static const std::thread::id theMainThread;
	static time_t startRunTime;
};
NAMESPACE_END_CHE
#endif // HCoreApplication_H__
