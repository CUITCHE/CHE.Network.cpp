#include "HData.h"
CHE_NAMESPACE_BEGIN

__memory__pool<__data__memory>* HData::__pool__ = new __memory__pool<__data__memory>;

HData::HData()
{

}

HData::~HData()
{

}

void HData::resetNextObjectPointer()
{
	__pointer__ = __data__header;
}

bool HData::next()
{
	return __pointer__ != nullptr;
}

__data__memory * HData::Object()
{
	auto p = __pointer__;
	__pointer__ = __pointer__->next;
	return p;
}


CHE_NAMESPACE_END