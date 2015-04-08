#include "HTimeC.h"

CHE_NAMESPACE_BEGIN
HTimeC::HTimeC(_hour h, _minu m, _seco s)
{
	hour(h), minute(m), second(s);
}

che::_hour HTimeC::hour() const
{
	return (_hour)(d_time & 0xFF0000);
}

_minu HTimeC::minute() const
{
	return (_minu)(d_time & 0xFF00);
}

_seco HTimeC::second() const
{
	return (_seco)(d_time & 0xFF);
}

bool HTimeC::hour(_hour h)
{
	return h >= 0 && h <= 23 ? d_time |= ((uint32)h << 16), true : false;
}

bool HTimeC::minute(_minu m)
{
	return m >= 0 && m <= 59 ? d_time |= ((uint32)m << 8), true : false;
}

bool HTimeC::second(_seco s)
{
	return s >= 0 && s <= 59 ? d_time |= ((uint32)s), true : false;
}

CHE_NAMESPACE_END
