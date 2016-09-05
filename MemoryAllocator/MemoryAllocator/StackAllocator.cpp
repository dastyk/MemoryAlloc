#include "StackAllocator.h"



StackAllocator::StackAllocator(size_t stackSize) : _stackSize(stackSize), _marker(0)
{
	_stack = operator new(stackSize);
}

StackAllocator::~StackAllocator()
{
	operator delete(_stack);
}

