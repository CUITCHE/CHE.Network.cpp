#ifndef ProtocolType_h__
#define ProtocolType_h__

#pragma once
/********************************************************************
	Copyright (C) 2014 CUIT��CHE.
	All rights reserved.
	Contact: CHE (hejun_753@163.com)

	created:	2014/02/08
	created:	8:2:2014   20:01
	filename: 	e:\documents\visual studio 2013\Projects\HSystem\HSystem\ProtocolType.h
	file path:	e:\documents\visual studio 2013\Projects\HSystem\HSystem
	file base:	ProtocolType
	file ext:	h
	author:		Jq
	
	purpose:	Э������
*********************************************************************/
#include "hglobal.h"
CHE_NAMESPACE_BEGIN
class ProtocolType
{
public:
	ProtocolType();
	~ProtocolType(){}
	enum _ProtocolType{
		//������Сֵ
		MIN = -32768,
		//�������ֵ
		MAX = 65535,
		//���ǰ�
		ISNOTPACKET = -2,
		//������
		HEARTCHECK = -1,
		//��Ǵ˰�Ϊ�հ�
		THISISEMPTY = 0,
		//�û���½
		ENTER = 1,
		//������Ϣ
		CHATINFO = 2,
		//��ȷ��Э��
		OPERATIONSURE = 3,
		//���������
		ERRORBACK = 4,
		//ע����Ϣ
		REGISTER = 5,
		/*���� Ϊ����ʵ��2 ר��*/
		//ͼƬ����
		SHAREPICTURE = 6,
		//��ȡͼƬ
		GETPICTURE = 7,
		//ͼƬ����
		CONTENTPICTURE = 8,
		//ͼƬ����
		LUADPICTURE = 9,
		//ͼƬ����
		IIPHOTOBINDATAPACKET = 10
	};
};
typedef ProtocolType HType;//Э������
CHE_NAMESPACE_END
#endif // ProtocolType_h__
