#include "MemoryManager.h"
#include <iostream>

MemoryManager::MemoryManager(uint32_t size)
{
	_free = _memory = new char[size];
	_remainingMemory = size;
}

MemoryManager::~MemoryManager()
{
	delete[] _memory;
}

PoolAllocator * MemoryManager::CreatePoolAllocator(uint32_t sizeOfObject, uint32_t nrOfObjects)
{
	_mutexLock.lock();
	uint64_t alignment = (uint64_t)__max(8, powl(2, ceil(log2l((long double)sizeOfObject))));
	
	char* rawAddress = reinterpret_cast<char*>(_free) + sizeof(PoolAllocator);
	uint64_t mask = alignment - 1;
	uint64_t misalignment = mask & (size_t)rawAddress;
	uint64_t adjustment = alignment - misalignment;
	uint64_t requirement = alignment*nrOfObjects + adjustment + sizeof(PoolAllocator);
	if (_remainingMemory < (requirement))
	{
		throw std::exception("Not enough memory");
	}
	_remainingMemory -= requirement;
	char* alignedAddress = rawAddress + adjustment;
	PoolAllocator* pool = new(_free) PoolAllocator((char*)alignedAddress, alignment, nrOfObjects);
	_free += requirement;
	_mutexLock.unlock();
	return pool;
}

StackAllocator * MemoryManager::CreateStackAllocator(uint64_t size)
{
	_mutexLock.lock();
	if (_remainingMemory < size + sizeof(StackAllocator))
	{
		throw std::exception("Not enough memory");
	}

	StackAllocator* stack = new(_free) StackAllocator(_free + sizeof(StackAllocator), size);
	_remainingMemory -= sizeof(StackAllocator) + size;
	_free += sizeof(StackAllocator) + size;
	_mutexLock.unlock();
	return stack;
}
