#include "StackAllocator.h"

#include <stdexcept>

StackAllocator::StackAllocator(size_t stackSize) : _stackSize(stackSize)
{
	_stack = operator new(stackSize);
	_marker = 0;
}

StackAllocator::~StackAllocator()
{
	operator delete(_stack);
}

void * StackAllocator::Alloc(size_t size)
{
	if (_marker + size > _stackSize)
		throw std::runtime_error("Stack allocator out of memory.");
	
	size_t memloc = _marker;
	_marker += size;
	
	return reinterpret_cast<char*>(_stack) + memloc;


}

void StackAllocator::FreeToMarker(size_t marker)
{
	_marker = marker;
}

void StackAllocator::Reset()
{
	_marker = 0;
}

size_t StackAllocator::GetMarker()
{
	return _marker;
}
