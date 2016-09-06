#include "StackAllocatorLock.h"



StackAllocatorLock::StackAllocatorLock(size_t stackSize) : _stackSize(stackSize), _marker(0)
{
	_stack = operator new(stackSize);
}

StackAllocatorLock::~StackAllocatorLock()
{
	operator delete(_stack);
}

