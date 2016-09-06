#ifndef STACK_ALLOCATORLOCK_H
#define STACK_ALLOCATORLOCK_H
#include <stdexcept>
#include <mutex>

class StackAllocatorLock
{
public:
	StackAllocatorLock(size_t stackSize);
	~StackAllocatorLock();

	inline void* Alloc(size_t size)
	{
		alloc_lock.lock();
		if (_marker + size > _stackSize)
			throw std::runtime_error("Stack allocator out of memory.");

		size_t memloc = _marker;
		_marker += size;
		alloc_lock.unlock();

		return reinterpret_cast<char*>(_stack) + memloc;


	}

	inline void FreeToMarker(size_t marker)
	{
		_marker = marker;
	}
	inline void Reset()
	{
		_marker = 0;
	}
	inline size_t GetMarker()
	{
		return _marker;
	}

private:
	size_t _stackSize;
	size_t _marker;

	void* _stack;
	std::mutex alloc_lock;

};


#endif //STACK_ALLOCATOR_H

