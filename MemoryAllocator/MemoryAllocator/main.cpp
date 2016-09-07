#include "TestCaseController.h"

#include "Timer.h"
#include "PoolAllocator.h"
#include "MemoryManager.h"
#include <iostream>
#include "TestStructs.h"
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
int main()
{
	/*TestCaseC test;

	test.TestPoolAllocatorThreaded<Enemies>();
	test.TestStackAllocatorThreaded<Enemies>();
	test.TestPoolAllocator<Enemies>();
	test.TestStackAllocator<Enemies>();*/


	MemoryManager m(2U * 1024U * 1024U * 1024U);
	PoolAllocator* pool = m.CreatePoolAllocator(sizeof(uint64_t), 1000000);

   uint64_t* i = (uint64_t*)pool->Malloc();
	*i = 5;

   uint64_t& a = *(uint64_t*)pool->Malloc();

	a = 6;

	std::cout << "i: " << *i << std::endl << "a: " << a << std::endl;
	

	system("pause");
	return 0;
}