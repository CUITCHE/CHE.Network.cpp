#ifndef HDataBufferdefs_H__
#define HDataBufferdefs_H__

/********************************************************************
	created:	2014/11/16
	created:	16:11:2014   19:27
	file base:	HDataBufferdefs
	author:		CHE
	
	purpose:	HDataBuffer����ض���
*********************************************************************/
#include "hglobal.h"
CHE_NAMESPACE_BEGIN

#define PACKETGUID_BEGIN	0	//Ψһ��ʶ����ʼ
#define PACKETGUID_END		7	//Ψһ��ʶ����ֹ

#define PROTOCOL_BEGIN		8	//Э����ʼ
#define PROTOCOL_END		9	//Э����ֹ

#define PACKETSERIAL_BEGIN	10	//�������ʼ
#define PACKETSERIAL_END	17	//�������ֹ

#define DATA_BEGIN			18	//������ʼ
const int EachPacketSize = 8 * 4;
#ifndef EACHPACKETSIZE
#define EACHPACKETSIZE EachPacketSize
#endif // !EACHPACKETSIZE

#define VERSION_1_0_2

#ifdef VERSION_1_0_2
/**
*	���ݰ��� - ver1.0.2
*	���ݰ���guid��8��+Э�顾2��+����š�8��+���ݡ�EachPacketSize-8-2-8�� = EachPacketSize
*	ÿ�����ݰ�Ϊ�̶�����4kb��Ϊ��ӭ��iocp�Ļ������Ĵ�С������
*	ע�ⷢ�Ͱ��ͽ��հ������𣬷��Ͱ���Ϊ�˱�֤ÿ�������ܷ���ȥ������Ϊ4kb�����հ��������з��Ͱ���۶���
*/
const int PACKETDATA_BEGIN = DATA_BEGIN;		//�����������������ݰ�����Ķ���
const int PACKETFETCHNUMS = EachPacketSize - DATA_BEGIN;		//ÿ����ȡ������
#endif // VERSION_1_0_2

CHE_NAMESPACE_END
#endif // HDataBufferdefs_H__
