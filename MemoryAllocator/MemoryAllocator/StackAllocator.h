#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

class StackAllocator
{
public:
	StackAllocator(size_t stackSize);
	~StackAllocator();

	void* Alloc(size_t size);
	void FreeToMarker(size_t marker);
	size_t GetMarker();
private:
	size_t _stackSize;
	size_t _marker;

	void* _stack;

};


#endif //STACK_ALLOCATOR_H
