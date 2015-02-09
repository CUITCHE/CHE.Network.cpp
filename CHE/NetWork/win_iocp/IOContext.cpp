#include "IOContext.h"

NAMESPACE_BEGIN_CHE

IOContext::IOContext()
:m_wsaBuf({ MAX_BUFFER_LEN, m_szBuffer })
{
	ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
	ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
}

IOContext::~IOContext()
{
	closeSocket(m_sockAccept);
}

NAMESPACE_END_CHE