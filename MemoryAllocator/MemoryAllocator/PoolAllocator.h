#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
#include <memory>
#include <stdexcept>

class PoolAllocator
{
private:
	struct List
	{
		uint32_t next;
	};
public:
	PoolAllocator(char* poolStart, size_t blocksize, size_t numBlocks);
	~PoolAllocator();

	inline void* Malloc()
	{
		if (_free == -1) throw std::runtime_error("Pool allocator out of memory.");
		
		uint32_t ret = _free; // Make copy for return.
		_free = _GetAt(_free)->next; // Copy the value that _free points to, to the variable _free.
		
		return _pool + ret;
	}
	inline void Free(char*p)
	{
		if (!p) throw std::runtime_error("Free on nullptr");
		
		uint32_t prev = _free; // Make copy of previous free block.
		_free = (uint32_t) (p - _pool); // Free the given block and set it as next free block.	
		_GetAt(_free)->next = prev; // Set the previous next free block as the next next free block.
		
	}
  
  

private:
   inline List* _GetAt(uint32_t offset)
   {
      return (List*)(_pool + offset);
   }
	void _SetupFreeBlocks();

private:
	char* _pool;
   uint32_t _free;
	size_t _numBlocks;
	size_t _blockSize;
};

#endif //POOL_ALLOCATOR_H
