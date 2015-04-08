#include "NetIOBuffer.h"
CHE_NAMESPACE_BEGIN


NetIOBuffer::~NetIOBuffer()
{
	che_array_pointer_del(buffer);
}

NetIOBuffer::NetIOBuffer(NetIOBuffer && rhs) 
	:s(rhs.s)
	,buffer(rhs.buffer)
{
	rhs.buffer = nullptr;
}

char * NetIOBuffer::buf()
{
	if (buffer == nullptr) {
		buffer = new char[MAX_BUFFER_LEN];
	}
	return buffer;
}

CHE_NAMESPACE_END
