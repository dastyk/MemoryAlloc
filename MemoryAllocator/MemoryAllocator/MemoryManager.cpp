#include "MemoryManager.h"
#include <iostream>

MemoryManager::MemoryManager(uint32_t size)
{
	_free = _memory = new char[size];
	_remainingMemory = size;
	_firstFree = (BlockInfo*)_free;
	_firstFree->size = size;
	_firstFree->next = nullptr;
}

MemoryManager::~MemoryManager()
{
	delete[] _memory;
}

PoolAllocator * MemoryManager::CreatePoolAllocator(uint32_t sizeOfObject, uint32_t nrOfObjects, uint8_t flag)
{
	if (flag & POOL_ALLOCATOR_ALIGNED)
	{
		_mutexLock.lock();
		uint64_t alignment = (uint64_t)__max(8, powl(2, ceil(log2l((long double)sizeOfObject))));

		//char* rawAddress = _free + sizeof(PoolAllocator);
		char* rawAddress = (char*)_Allocate(alignment * nrOfObjects + sizeof(PoolAllocator) + alignment);
		uint64_t mask = alignment - 1;
		uint64_t misalignment = mask & (size_t)rawAddress;
		uint64_t adjustment = alignment - misalignment;
		uint64_t requirement = alignment * nrOfObjects + sizeof(PoolAllocator) + alignment;
		if (_remainingMemory < (requirement))
		{
			throw std::exception("Not enough memory");
		}
		_remainingMemory -= requirement;
		char* alignedAddress = rawAddress + adjustment + sizeof(PoolAllocator);
		PoolAllocator* pool = new(rawAddress) PoolAllocator(alignedAddress, alignment, nrOfObjects);
		_allocatedBlocks[rawAddress] = alignment * nrOfObjects + sizeof(PoolAllocator) + alignment;
		_mutexLock.unlock();
		return pool;
	}
	else
	{
		char* rawAdd = (char*)_Allocate(sizeOfObject * nrOfObjects + sizeof(PoolAllocator));
		PoolAllocator* pool = new(rawAdd) PoolAllocator(rawAdd + sizeof(PoolAllocator), sizeOfObject, nrOfObjects);
		_allocatedBlocks[rawAdd] = sizeof(PoolAllocator) + sizeOfObject * nrOfObjects;
		return pool;
	}
}

StackAllocator * MemoryManager::CreateStackAllocator(uint64_t size)
{
	_mutexLock.lock();
	if (_remainingMemory < size + sizeof(StackAllocator))
	{
		throw std::exception("Not enough memory");
	}
	char* rawAdd = (char*)_Allocate(size + sizeof(StackAllocator));
	StackAllocator* stack = new(rawAdd) StackAllocator(rawAdd + sizeof(StackAllocator), size);
	_allocatedBlocks[rawAdd] = sizeof(StackAllocator) + size;
	_remainingMemory -= sizeof(StackAllocator) + size;
	_mutexLock.unlock();
	return stack;
}

void MemoryManager::ReleasePoolAllocator(PoolAllocator * object)
{
	_Free(object, _allocatedBlocks[(void*)object]);
	_remainingMemory += _allocatedBlocks[object];
	_allocatedBlocks.erase(object);
}

void MemoryManager::ReleaseStackAllocator(StackAllocator * object)
{
	_Free(object, _allocatedBlocks[(void*)object]);
	_remainingMemory += _allocatedBlocks[object];
	_allocatedBlocks.erase(object);
}

void MemoryManager::PrintBlockInfo()
{
	BlockInfo* b = _firstFree;
	std::cout << "Starting address: " << reinterpret_cast<uintptr_t>(_memory) << "\n";
	do {
		std::cout << "Free Block @ " << reinterpret_cast<uintptr_t>(b) << ".   Size: " << b->size << std::endl;
		b = b->next;
	} while (b);

	for (auto& i : _allocatedBlocks)
	{
		std::cout << "Allocated Block @ " << reinterpret_cast<uintptr_t>(i.first) << ".    Size: " << i.second << std::endl;
	}
	std::cout << "Remaining memory: " << _remainingMemory << "\n";
}
