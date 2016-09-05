#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
#include <memory>
#include <stdexcept>
#include <mutex>
class PoolAllocator
{
private:
	struct List
	{
		void* next;
	};
public:
	PoolAllocator(size_t blocksize, size_t numBlocks);
	~PoolAllocator();

	inline void* Malloc()
	{
		if (!_free) throw std::runtime_error("Pool allocator out of memory.");
		alloc_lock.lock();
		void* ret = _free; // Make copy for return.
		_free = ((List*)_free)->next; // Copy the value that _free points to, to the variable _free.
		alloc_lock.unlock();
		return ret;
	}
	inline void Free(void*p)
	{
		if (!p) throw std::runtime_error("Free on nullptr");
		alloc_lock.lock();
		void* prev = _free; // Make copy of previous free block.
		_free = p;// Free the given block and set it as next free block.
		((List*)_free)->next = prev; // Set the previous next free block as the next next free block.
		alloc_lock.unlock();
	}

private:
	void _SetupFreeBlocks();

private:
	void* _pool;
	void* _free;
	size_t _numBlocks;
	size_t _blockSize;
	std::mutex alloc_lock;
};

#endif //POOL_ALLOCATOR_H