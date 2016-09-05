#include "PoolAllocator.h"
#include <memory>


PoolAllocator::PoolAllocator(size_t blocksize, size_t numBlocks) : _blockSize(blocksize), _numBlocks(numBlocks)
{
	_pool = operator new(_blockSize * _numBlocks);

	_SetupFreeBlocks();
}


PoolAllocator::~PoolAllocator()
{
	operator delete(_pool);
}

inline void * PoolAllocator::Malloc()
{
	return nullptr;
}

inline void PoolAllocator::Free(void *)
{
}

void PoolAllocator::_SetupFreeBlocks()
{
	char* p = reinterpret_cast<char*>(_pool);
	char* nextPointer = p + _blockSize;

	// Iterate through blocks (all are free at first) and set the first bytes
	// to the pointer of the next block, thereby creating a linked list.
	for (uint32_t i = 0; i < _numBlocks; ++i)
	{
		memcpy(p, &nextPointer, sizeof(nextPointer));
		p += _blockSize;
	}

	_free = _pool;
}
