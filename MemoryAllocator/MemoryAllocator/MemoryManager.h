#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include "PoolAllocator.h"
#include "StackAllocator.h"
#include <mutex>
#include <map>

class MemoryManager
{
public:
	MemoryManager(size_t size);
	~MemoryManager();
private:
	std::mutex _mutexLock;
	char* _memory;
	char* _free;

	uint32_t _totalMemory;
	uint32_t _remainingMemory;

	struct UsedAllocatorStack
	{
		bool inUse = false;
		uint32_t size = 0;
		uint32_t blockSize = 0;
		uint32_t objectSize = 0;
		StackAllocator* stack;
	};



public:

	PoolAllocator* CreatePoolAllocator(uint32_t sizeOfObject, uint32_t nrOfObjects);
	StackAllocator* CreateStackAllocator(uint32_t size);

	void ReleasePoolAllocator(PoolAllocator* object);
	void ReleaseStackAllocator(StackAllocator* object);

};








#endif