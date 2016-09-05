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

inline void * PoolAllocator::Malloc()
{
	return nullptr;
}

inline void PoolAllocator::Free(void *)
{
}

void PoolAllocator::_SetupFreeBlocks()
{
	
}
