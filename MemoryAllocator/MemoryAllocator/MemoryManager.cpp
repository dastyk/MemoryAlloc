#include "MemoryManager.h"
#include <iostream>

MemoryManager::MemoryManager(uint32_t size)
{
	_free = _memory = new char[size];
	_totalMemory = _remainingMemory = size;
}

MemoryManager::~MemoryManager()
{
	delete[] _memory;
}

PoolAllocator * MemoryManager::CreatePoolAllocator(uint32_t sizeOfObject, uint32_t nrOfObjects)
{
	_mutexLock.lock();
	uint32_t alignment = (uint32_t)__max(8,powl(2,ceil(log2l((long double)sizeOfObject))));
	
	char* rawAddress = reinterpret_cast<char*>(_free) + sizeof(PoolAllocator);
	uint32_t mask = alignment - 1;
	uint32_t misalignment = mask & (size_t)rawAddress;
	uint32_t adjustment = alignment - misalignment;
	uint32_t requirement = alignment*nrOfObjects + adjustment + sizeof(PoolAllocator);
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

StackAllocator * MemoryManager::CreateStackAllocator(uint32_t size)
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
