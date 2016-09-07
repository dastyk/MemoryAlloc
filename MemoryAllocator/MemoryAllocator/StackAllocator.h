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

		size_t stackPos = _marker;
		_marker += size;

		return reinterpret_cast<char*>(_stack) + stackPos;


	}
	
	inline void* AllocAligned(size_t size, uint32_t alignment)
	{
		//Here's the code that is actually readable
		/*
		uint32_t mask = alignment - 1;
		size_t address = reinterpret_cast<uintptr_t>(_stack) + _marker;
		uint32_t misalignment = mask & address;
		uint32_t adjustment = alignment - misalignment;
		if (_marker + size + adjustment > _stackSize)
			throw std::runtime_error("Stack allocator out of memory.");
		size_t stackPos = _marker + adjustment;
		_marker += size + adjustment;
		return reinterpret_cast<char*>(_stack) + stackPos;
		*/
		//Here's the same code as above but declaring only 1 variable because possibly marginally faster
		size_t address = (reinterpret_cast<uintptr_t>(_stack) + _marker) & (alignment - 1);
		address = alignment - address + _marker;
		if(address + size > _stackSize)
			throw std::runtime_error("Stack allocator out of memory.");
		_marker = address;
		return reinterpret_cast<char*>(_stack) + address;

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
