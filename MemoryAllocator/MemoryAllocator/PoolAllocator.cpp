#include "PoolAllocator.h"
#include <memory>
#include <stdexcept>

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
	if (!_free) throw std::runtime_error("No more memory.");

	void* ret = _free; // Make copy for return.
	memcpy(&_free, _free, sizeof(void*)); // Copy the value that _free points to, to the variable _free.
	return ret;
}

inline void PoolAllocator::Free(void * p)
{
	if (!p) throw std::runtime_error("Free on nullptr");
	void* prev = _free; // Make copy of previous free block.
	memcpy(&_free, p, sizeof(void*)); // Free the given block and set it as next free block.
	memcpy(_free, &prev, sizeof(void*)); // Set the previous next free block as the next next free block.
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
