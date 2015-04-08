#include "../NetWork/socket/private/HAbstractSocketPrivate.h"

CHE_NAMESPACE_BEGIN

HAbstractSocketPrivate::operator SOCKET() const
{
	return fd;
}

HAbstractSocketPrivate::HAbstractSocketPrivate(const SOCK_TYPE type)
	:socket_type(type)
{
}

CHE_NAMESPACE_END
