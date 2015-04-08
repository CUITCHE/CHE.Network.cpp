#include "SocketContext.h"
#include "IOContext.h"
#include <algorithm>
#include <cassert>
CHE_NAMESPACE_BEGIN

SocketContext::SocketContext()
{
	memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
}

SocketContext::~SocketContext()
{
	closeSocket(m_Socket);
	auto iters = m_arrayIoContext.begin();
	auto itere = m_arrayIoContext.end();
	IOContext *pf = nullptr;
	while (iters != itere) {
		pf = *iters;
		delete pf;
		++iters;
	}
}

IOContext* SocketContext::getOneIOContext()
{
	IOContext	*p(new IOContext);

	m_arrayIoContext.push_back(p);

	return p;
}

void SocketContext::removeOneContext(IOContext *pContext)
{
	assert(pContext != NULL);
	std::remove(m_arrayIoContext.begin(), m_arrayIoContext.end(), pContext);
	delete pContext;
}

CHE_NAMESPACE_END
