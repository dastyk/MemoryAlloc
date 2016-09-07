#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
#include <memory>
#include <stdexcept>

class PoolAllocator
{
private:
	struct List
	{
		char* next;
	};

public:
	PoolAllocator(char* poolStart, size_t blocksize, size_t numBlocks);
	~PoolAllocator();

	inline void* Malloc()
	{
		if (!_freeBlockList) throw std::runtime_error("Pool allocator out of memory.");

      // Set the next free block to be the second one in the list and return
      // the original free block (effectively consuming the first block).
		char* ret = _freeBlockList;
      _freeBlockList = ((List*)_freeBlockList)->next;
		
		return ret;
	}

	inline void Free(char*p)
	{
		if (!p) throw std::runtime_error("Free on nullptr");

      // When freeing a block, we put it first in the list of empty blocks.
      // To not sever the link we make it point to the previous head.
		char* prev = _freeBlockList;
      _freeBlockList = p;
		((List*)_freeBlockList)->next = prev;
	}  

private:
	void _SetupFreeBlocks();

private:
	char* _pool;
   char* _freeBlockList;
	size_t _numBlocks;
	size_t _blockSize;
};

#endif //POOL_ALLOCATOR_H
