#include "PoolAllocator.h"


PoolAllocator::PoolAllocator(char* poolStart, size_t blocksize, size_t numBlocks) : _blockSize(blocksize), _numBlocks(numBlocks), _pool(poolStart)
{
   if (blocksize < sizeof(_pool))
   {
      throw std::runtime_error("Block size must be larger than the size of a pointer!");
   }

   // Sets up the free blocks to forms a linked list
	_SetupFreeBlocks();
}


PoolAllocator::~PoolAllocator()
{
}


void PoolAllocator::_SetupFreeBlocks()
{
   _freeBlockList = _pool;

	// Iterate through blocks (all are free at first) and set the first bytes
	// to the pointer of the next block, thereby creating a linked list.
	char* p = _pool;
	for (size_t i = 0; i < _numBlocks - 1; ++i)
	{
		((List*)p)->next = p + _blockSize;
		p += _blockSize;
	}

   // The last block does not have a next free block, and this is indicated by
   // using a nullptr.
   ((List*)p)->next = nullptr;
}
