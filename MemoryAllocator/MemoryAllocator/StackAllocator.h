#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H
#include <stdexcept>
class StackAllocator
{
public:
	StackAllocator(size_t stackSize);
	~StackAllocator();

	inline void* Alloc(size_t size)
	{
		if (_marker + size > _stackSize)
			throw std::runtime_error("Stack allocator out of memory.");

		size_t memloc = _marker;
		_marker += size;

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

};


#endif //STACK_ALLOCATOR_H
