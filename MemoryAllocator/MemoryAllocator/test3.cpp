#include "MemoryManager.h"
#include "Timer.h"
#include <stdint.h>
#include <iostream>
#include <random>
static MemoryManager* _memoryManager = new MemoryManager(2U*1024U*1024U*1024U);
#define NR_OF_TESTS 100

struct Data
{
	uint32_t m[16];
};
	int *randomsEachFrame;
	void RandomizeTheRandom()
	{
		for (int i = 0; i < NR_OF_TESTS; i++)
		{
			int random1 = rand() % NR_OF_TESTS;
			int random2 = rand() % NR_OF_TESTS;
			int temp = randomsEachFrame[random1];
			randomsEachFrame[random1] = randomsEachFrame[random2];
			randomsEachFrame[random2] = temp;
		}
	}

template <typename T>
uint64_t TestRandomNewDeletePool(uint32_t allign)
{
	
	return totalTime;
}

int main()
{

	Timer timer;
	PoolAllocator *testAllocator = _memoryManager->CreatePoolAllocator(sizeof(T), NR_OF_TESTS, allign);
	T **testData = new T*[NR_OF_TESTS];
	timer.Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		testData[i] = (T*)testAllocator->Malloc();
	}
	for (int i = 0; i < NR_OF_TESTS / 10; i++)
	{
		testAllocator->Free((char*)testData[randomsEachFrame[i]]);
	}
	for (int i = NR_OF_TESTS / 10; i > 0; i--)
	{
		testData[randomsEachFrame[i]] = (T*)testAllocator->Malloc();
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		testAllocator->Free((char*)testData[i]);
	}
	uint64_t totalTime = timer.Elapsed().count();
	delete[] testData;
	_memoryManager->ReleasePoolAllocator(testAllocator);
	std::cout << "Time: " << TestRandomNewDeletePool<uint32_t>(16) << std::endl;

	return 0;
}