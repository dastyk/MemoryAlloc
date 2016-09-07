#include "MemoryManager.h"

MemoryManager::MemoryManager(size_t size)
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
	size_t alignment = powl(2,ceil(log2l(sizeOfObject)));
	
	char* rawAddress = reinterpret_cast<char*>(_free) + sizeof(PoolAllocator);
	size_t mask = alignment - 1;
	uint32_t misalignment = mask & (size_t)rawAddress;
	uint32_t adjustment = alignment - misalignment;
	size_t requirement = alignment*nrOfObjects + adjustment + sizeof(PoolAllocator);
	if (_remainingMemory < (requirement))
	{
		throw std::exception("Not enough memory");
	}
	_remainingMemory -= requirement;
	char* alignedAddress = rawAddress + adjustment;
	PoolAllocator* pool = new(_free) PoolAllocator((char*)alignedAddress, alignment, nrOfObjects);
	_free += requirement;
	return pool;
}

StackAllocator * MemoryManager::CreateStackAllocator(uint32_t size)
{
	return nullptr;
}
