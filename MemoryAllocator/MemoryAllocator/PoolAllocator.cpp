#include "PoolAllocator.h"


PoolAllocator::PoolAllocator(size_t blocksize, size_t numBlocks) : _blockSize(blocksize), _numBlocks(numBlocks)
{
	_pool = operator new(_blockSize * _numBlocks);

	_SetupFreeBlocks();
}


PoolAllocator::~PoolAllocator()
{
	operator delete(_pool);
}


void PoolAllocator::_SetupFreeBlocks()
{
	char* p = reinterpret_cast<char*>(_pool);

	// Iterate through blocks (all are free at first) and set the first bytes
	// to the pointer of the next block, thereby creating a linked list.
	for (uint32_t i = 0; i < _numBlocks-1; ++i)
	{
		char* nextPointer = p + _blockSize;
		memcpy(p, &nextPointer, sizeof(nextPointer));
		p += _blockSize;
	}
	char* nextPointer = nullptr;
	memcpy(p, &nextPointer, sizeof(nextPointer));
	_free = _pool;
}
