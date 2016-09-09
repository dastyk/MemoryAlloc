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

public:

	PoolAllocator* CreatePoolAllocator(uint32_t sizeOfObject, uint32_t nrOfObjects, uint8_t flag = POOL_ALLOCATOR_ALIGNED);
	StackAllocator* CreateStackAllocator(uint64_t size);

	//void ReleasePoolAllocator(PoolAllocator* object);
	//void ReleaseStackAllocator(StackAllocator* object);

};








#endif