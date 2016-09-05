#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
#include <memory>
#include <stdexcept>
class PoolAllocator
{
public:
	PoolAllocator(size_t blocksize, size_t numBlocks);
	~PoolAllocator();

	inline void* Malloc()
	{
		if (!_free) throw std::runtime_error("No more memory.");

		void* ret = _free; // Make copy for return.
		memcpy(&_free, _free, sizeof(void*)); // Copy the value that _free points to, to the variable _free.
		return ret;
	}
	inline void Free(void*p)
	{
		if (!p) throw std::runtime_error("Free on nullptr");
		void* prev = _free; // Make copy of previous free block.
		memcpy(&_free, &p, sizeof(void*)); // Free the given block and set it as next free block.
		memcpy(_free, &prev, sizeof(void*)); // Set the previous next free block as the next next free block.
	}

private:
	void _SetupFreeBlocks();

private:
	void* _pool;
	void* _free;
	size_t _numBlocks;
	size_t _blockSize;
};

#endif //POOL_ALLOCATOR_H