#include "PoolAllocator.h"


PoolAllocator::PoolAllocator(char* poolStart, size_t blocksize, size_t numBlocks) : _blockSize(blocksize), _numBlocks(numBlocks), _pool(poolStart)
{
	_SetupFreeBlocks();
}


PoolAllocator::~PoolAllocator()
{
}


void PoolAllocator::_SetupFreeBlocks()
{
	char* p = _pool;

	// Iterate through blocks (all are free at first) and set the first bytes
	// to the pointer of the next block, thereby creating a linked list.
	for (size_t i = 0; i < _numBlocks - 1; ++i)
	{
		((List*)p)->next = p + _blockSize;
		p += _blockSize;
	}
   ((List*)p)->next = nullptr;
	_free = _pool;
}
