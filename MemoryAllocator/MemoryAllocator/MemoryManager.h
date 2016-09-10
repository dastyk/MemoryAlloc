#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include "PoolAllocator.h"
#include "StackAllocator.h"
#include <mutex>
#include <map>

#define POOL_ALLOCATOR_ALIGNED 1 << 0
#define POOL_ALLOCATOR_UNALIGNED 1 << 1

class MemoryManager
{
public:
	MemoryManager(uint32_t size);
	~MemoryManager();
private:
	std::mutex _mutexLock;
	char* _memory;
	char* _free;

	uint64_t _remainingMemory;

	std::map<void*, size_t> _allocatedBlocks; //maps address to size of block

	struct BlockInfo
	{
		BlockInfo(size_t size, BlockInfo* next = nullptr) : size(size), next(next) {}
		size_t size;
		BlockInfo* next;
	};

	BlockInfo* _firstFree;

	inline void* _Allocate(size_t size)
	{
		BlockInfo* freeBlock = _firstFree;
		BlockInfo* previous = nullptr;
		while (freeBlock->size < size && freeBlock->next != nullptr)
		{
			previous = freeBlock;
			freeBlock = freeBlock->next;
		}
		if (freeBlock->size > size + sizeof(BlockInfo))
		{
			if (freeBlock == _firstFree)
			{
				BlockInfo old = *_firstFree;
				_firstFree = (BlockInfo*)((char*)_firstFree + size);
				_firstFree->size = old.size - size;
				_firstFree->next = old.next;
			}
			else
			{
				previous->next = (BlockInfo*)((char*)freeBlock + size);
				previous->next->size = freeBlock->size - size;
				previous->next->next = freeBlock->next;
			}
			return freeBlock;
		}
		throw std::runtime_error("Out of memory");
	}

	inline void _Free(void* address, size_t size)
	{
		//Check if free block has any free "neighbours"
		BlockInfo* left = nullptr;
		BlockInfo* right = nullptr;
		BlockInfo* free = _firstFree;
		while (free->next)
		{
			if ((char*)free + free->size == address)
				left = free;
			else if ((char*)free == (char*)address + size)
				right = free;
			free = free->next;
		}
		if (left && right)
		{
			left->size += size + right->size;
			if (left->next == right)
				left->next = right->next;
		}
		else if (left)
		{
			left->size += size;
		}
		else if (right)
		{
			//Find which block points to "right" and adjust it
			BlockInfo* b = _firstFree;
			if (b == _firstFree)
			{
				_firstFree = (BlockInfo*)((char*)_firstFree - size);
				_firstFree->size = b->size + size;
				_firstFree->next = b->next;
			}
			else
			{
				while (b->next && b->next != right)
				{
					b = b->next;
				}
				b->next = right->next;
				b->size = right->size + size;
			}
		}
		else
		{
			//Find last free block and point it to this address
			BlockInfo* here = (BlockInfo*)address;
			here->next = nullptr;
			here->size = size;
			BlockInfo* iter = _firstFree;
			while (iter->next != nullptr)
				iter = iter->next;
			iter->next = here;
		}
	}



public:

	PoolAllocator* CreatePoolAllocator(uint32_t sizeOfObject, uint32_t nrOfObjects, uint8_t flag = POOL_ALLOCATOR_ALIGNED);
	StackAllocator* CreateStackAllocator(uint64_t size);

	void ReleasePoolAllocator(PoolAllocator* object);
	void ReleaseStackAllocator(StackAllocator* object);

};








#endif