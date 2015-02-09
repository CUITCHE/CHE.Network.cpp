//
#include "HDataBufferPartionCollection.h"

using std::pair;
NAMESPACE_BEGIN_CHE

HDataBufferPartionCollection::HDataBufferPartionCollection(size_t size)
	:size(size)
{
}

bool HDataBufferPartionCollection::append(int order, shared_ptr<HDataBufferPartion> append_data)
{
	if (data_map.size() >= size)	return false;

	auto iter = data_map.find(order);
	if (iter != data_map.end()){
		iter->second = append_data;
	}
	else{
		data_map.insert(pair<int, shared_ptr<HDataBufferPartion>>(order, append_data));
	}

	return true;
}
NAMESPACE_END_CHE