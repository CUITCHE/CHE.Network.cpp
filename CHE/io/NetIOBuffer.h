#ifndef NetIOBuffer_H__
#define NetIOBuffer_H__

/********************************************************************
	created:	2014/12/01
	created:	1:12:2014   16:00
	file base:	NetIOBuffer
	author:		CHE
	
	purpose:	����io��������ר��iocpʹ�ã�ÿ��buf�̶���С
*********************************************************************/
#include "hglobal.h"
#include "HIOCPdefs.h"

CHE_NAMESPACE_BEGIN

class NetIOBuffer
{
	CHE_DISABLE_COPY(NetIOBuffer);
public:
	NetIOBuffer():buffer(nullptr),s(INVALID_SOCKET){}
	~NetIOBuffer();
	NetIOBuffer(NetIOBuffer &&rhs);
	void operator=(NetIOBuffer &&rhs) {
		s = rhs.s;
		che_array_pointer_del(buffer);
		std::swap(buffer, rhs.buffer);
	}
	char *buf();
	SOCKET s;
private:
	char *buffer;
};
CHE_NAMESPACE_END
#endif // NetIOBuffer_H__
