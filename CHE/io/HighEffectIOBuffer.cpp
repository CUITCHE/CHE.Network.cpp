#include "HighEffectIOBuffer.h"

CHE_NAMESPACE_BEGIN


HighEffectIOBuffer::HighEffectIOBuffer()
:d_ptr(new HighEffectIOBufferData())
{
}

HighEffectIOBuffer::~HighEffectIOBuffer()
{
	
}

bool HighEffectIOBuffer::nextpadding() const
{
	return d_ptr->used->size() != 0;
}

int HighEffectIOBuffer::unused_size() const
{
	return d_ptr->unused->size();
}

void HighEffectIOBuffer::buffer_used(NetIOBuffer *buffer)
{
	std::lock_guard<mutex> locker(*d_ptr->read_for_used);
	*buffer = (move_quick(d_ptr->used->front()));
	d_ptr->used->pop_front();
}

void HighEffectIOBuffer::buffer_used(NetIOBuffer &&buffer)
{
	buffer.s = INVALID_SOCKET;
	std::lock_guard<mutex> locker(*d_ptr->write_for_unused);
	d_ptr->unused->push_back(move_quick(buffer));
}

void HighEffectIOBuffer::buffer_unused(NetIOBuffer *buffer)
{
	std::lock_guard<mutex> locker(*d_ptr->write_for_unused);
	if (d_ptr->unused->size() == 0) {
		d_ptr->unused->resize(d_ptr->used->size());
	}

	*buffer = move_quick(d_ptr->unused->front());
	d_ptr->unused->pop_front();
}

void HighEffectIOBuffer::buffer_unused(NetIOBuffer &&buffer)
{
	
	std::lock_guard<mutex> locker(*d_ptr->read_for_used);
	d_ptr->used->push_back(move_quick(buffer));
}

CHE_NAMESPACE_END
