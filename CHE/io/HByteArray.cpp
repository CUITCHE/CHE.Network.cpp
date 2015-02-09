//
#include "HByteArray.h"
NAMESPACE_BEGIN_CHE
HByteArray::HByteArray(byte *array /*= NULL*/, size_t size /*= 0*/)
	:m_array(array), m_size(size)
{

}

HByteArray::~HByteArray()
{
	destroy();
}

void HByteArray::destroy()
{
	if (m_array == NULL)
		return;
	m_size > 1 ? delete[]m_array : delete m_array;
}

void HByteArray::reset(byte *array, size_t size)
{
	destroy();
	m_array = array;
	m_size = size;
}
NAMESPACE_END_CHE