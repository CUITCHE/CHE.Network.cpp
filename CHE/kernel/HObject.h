#ifndef HObject_h__
#define HObject_h__

/********************************************************************
Copyright (C) 2014 CUIT��CHE.
All rights reserved.
Contact: CHE (hejun_753@163.com)

created:	2014/02/08
created:	8:2:2014   19:31
purpose:	������Ļ���,������ʼ���չ���������ȥ�����������
			ʵ�����ڴ����,�����౻�ͷţ�����Ҳ������ͷš�
*********************************************************************/
#include "hglobal.h"
#include <memory>
using std::unique_ptr;
using std::string;
CHE_NAMESPACE_BEGIN

struct HObjectPrivate;
class HObject
{
	friend struct HObjectPrivate;
	CHE_PRIVATE_DECLARE(HObject)
public:
	HObject(HObject *parent = 0);
	virtual ~HObject();

	/*������ǰ�̣߳�ֱ��������ϣ���HCoreApplication������quit��̬����*/
	virtual int exec();

	/*���ر�ʾ��ǰ������ַ���*/
	virtual string to_string();

	/*ȷ��ָ���Ķ����Ƿ���ڵ�ǰ����
	��ȷ���true
	*/
	virtual bool equal(const HObject &obj)const;

	/*����ϵͳĬ�ϵĹ�ϣ���������ع�ϣֵ*/
	virtual size_t hash_code()const;
private:
	std::unique_ptr<HObjectPrivate> d_ptr;
};
CHE_NAMESPACE_END
#endif // HObject_h__