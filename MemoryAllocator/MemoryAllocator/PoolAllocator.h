#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
class PoolAllocator
{
public:
	PoolAllocator(size_t blocksize, size_t numBlocks);
	~PoolAllocator();

	inline void* Malloc();
	inline void Free(void*);

private:
	void _SetupFreeBlocks();

private:
	void* _pool;
	void* _free;
	size_t _numBlocks;
	size_t _blockSize;
};

#endif //POOL_ALLOCATOR_H