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
		if (!_free) throw std::runtime_error("Pool allocator out of memory.");
		
		char* ret = _free; // Make copy for return.
		_free = ((List*)_free)->next; // Copy the value that _free points to, to the variable _free.
		
		return ret;
	}
	inline void Free(char*p)
	{
		if (!p) throw std::runtime_error("Free on nullptr");
		
		char* prev = _free; // Make copy of previous free block.
		_free = p;// Free the given block and set it as next free block.
		((List*)_free)->next = prev; // Set the previous next free block as the next next free block.
		
	}

private:
	void _SetupFreeBlocks();

private:
	char* _pool;
	char* _free;
	size_t _numBlocks;
	size_t _blockSize;
};

#endif //POOL_ALLOCATOR_H
