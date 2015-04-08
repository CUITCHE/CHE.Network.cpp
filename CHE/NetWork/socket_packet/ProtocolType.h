#ifndef ProtocolType_h__
#define ProtocolType_h__

#pragma once
/********************************************************************
	Copyright (C) 2014 CUIT·CHE.
	All rights reserved.
	Contact: CHE (hejun_753@163.com)

	created:	2014/02/08
	created:	8:2:2014   20:01
	filename: 	e:\documents\visual studio 2013\Projects\HSystem\HSystem\ProtocolType.h
	file path:	e:\documents\visual studio 2013\Projects\HSystem\HSystem
	file base:	ProtocolType
	file ext:	h
	author:		Jq
	
	purpose:	协议类型
*********************************************************************/
#include "hglobal.h"
CHE_NAMESPACE_BEGIN
class ProtocolType
{
public:
	ProtocolType();
	~ProtocolType(){}
	enum _ProtocolType{
		//保留最小值
		MIN = -32768,
		//保留最大值
		MAX = 65535,
		//不是包
		ISNOTPACKET = -2,
		//心跳包
		HEARTCHECK = -1,
		//标记此包为空包
		THISISEMPTY = 0,
		//用户登陆
		ENTER = 1,
		//聊天信息
		CHATINFO = 2,
		//包确认协议
		OPERATIONSURE = 3,
		//错误包返回
		ERRORBACK = 4,
		//注册信息
		REGISTER = 5,
		/*以下 为工程实践2 专用*/
		//图片分享
		SHAREPICTURE = 6,
		//获取图片
		GETPICTURE = 7,
		//图片评论
		CONTENTPICTURE = 8,
		//图片点赞
		LUADPICTURE = 9,
		//图片数据
		IIPHOTOBINDATAPACKET = 10
	};
};
typedef ProtocolType HType;//协议类型
CHE_NAMESPACE_END
#endif // ProtocolType_h__
