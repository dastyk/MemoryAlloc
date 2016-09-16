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

// alignment is the desired alignment, i.e. blocks will start on a multiple of this.
// For example, one might want to store structs of size 136 (perhaps two matrices
// and two floats) with alignment 64 for the matrices. A value of zero means that
// we don't care about alignment.
PoolAllocator * MemoryManager::CreatePoolAllocator(uint32_t sizeOfObject, uint32_t nrOfObjects, uint32_t alignment)
{
	// Always use minimum size of 8 due to pointers in pool :/
	if (sizeOfObject < 8)
		sizeOfObject = 8;

	if (alignment)
	{
		_mutexLock.lock();

		// Use a block size that is the smallest multiple of the desired alignment
		// greater than or equal to the block size.
		uint32_t trailing = sizeOfObject % alignment;
		uint32_t effectiveBlockSize = sizeOfObject;
		if (trailing != 0)
		{
			// Enlarge to next multiple of alignment
			effectiveBlockSize += (alignment - trailing);
		}

		//char* rawAddress = _free + sizeof(PoolAllocator);
		char* rawAddress = (char*)_Allocate(effectiveBlockSize * (nrOfObjects + 1) + sizeof(PoolAllocator));
		uint64_t mask = alignment - 1;
		uint64_t misalignment = mask & (size_t)rawAddress;
		uint64_t adjustment = alignment - misalignment;
		uint64_t requirement = effectiveBlockSize * (nrOfObjects + 1) + sizeof(PoolAllocator);
		if (_remainingMemory < (requirement))
		{
			throw std::exception("Not enough memory");
		}
		_remainingMemory -= requirement;
		char* alignedAddress = rawAddress + adjustment + sizeof(PoolAllocator);
		PoolAllocator* pool = new(rawAddress) PoolAllocator(alignedAddress, effectiveBlockSize, nrOfObjects);
		_allocatedBlocks[rawAddress] = effectiveBlockSize * (nrOfObjects + 1) + sizeof(PoolAllocator);

		_mutexLock.unlock();

		return pool;
	}
	else
	{
		_mutexLock.lock();
		char* rawAdd = (char*)_Allocate(sizeOfObject * nrOfObjects + sizeof(PoolAllocator));
		PoolAllocator* pool = new(rawAdd) PoolAllocator(rawAdd + sizeof(PoolAllocator), sizeOfObject, nrOfObjects);
		_allocatedBlocks[rawAdd] = sizeof(PoolAllocator) + sizeOfObject * nrOfObjects;
		_mutexLock.unlock();
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
