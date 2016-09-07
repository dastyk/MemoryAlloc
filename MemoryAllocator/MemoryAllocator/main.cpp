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
	TestCaseC test;

	test.TestPoolAllocatorThreaded<Enemies>();
	test.TestStackAllocatorThreaded<Enemies>();
	test.TestPoolAllocator<Enemies>();
	test.TestStackAllocator<Enemies>();

	system("pause");
	return 0;
}