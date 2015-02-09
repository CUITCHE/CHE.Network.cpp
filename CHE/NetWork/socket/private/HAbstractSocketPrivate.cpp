#include "../NetWork/socket/private/HAbstractSocketPrivate.h"

NAMESPACE_BEGIN_CHE

HAbstractSocketPrivate::operator SOCKET() const
{
	return fd;
}

HAbstractSocketPrivate::HAbstractSocketPrivate(const SOCK_TYPE type)
	:socket_type(type)
{
}

NAMESPACE_END_CHE