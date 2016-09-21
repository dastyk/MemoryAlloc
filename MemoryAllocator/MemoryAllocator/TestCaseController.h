#ifndef _TEST_CASE_CONTROLLER_H
#define _TEST_CASE_CONTROLLER_H


#include "TestStructs.h"
#include <iostream>
#include <random>
#include <thread>
#include <future>
#include "Timer.h"
#include <vector>


#include "MemoryManager.h"


#define NR_OF_TESTS 5000000
#define NR_OF_THREADS 8
#define NR_OF_CACHE_TESTS 10000

struct matrix
{
	uint64_t a1 = 0, a2 = 0, a3 = 0, a4 = 0;
	uint64_t b1 = 0, b2 = 0, b3 = 0, b4 = 0;
	uint64_t c1 = 0, c2 = 0, c3 = 0, c4 = 0;
	uint64_t d1 = 0, d2 = 0, d3 = 0, d4 = 0;
};

struct matrix2
{
	uint64_t a1 = 0, a2 = 0, a3 = 0, a4 = 0;
	uint64_t b1 = 0, b2 = 0, b3 = 0, b4 = 0;
};

class TestCaseC
{
public:
	TestCaseC();
	~TestCaseC();

	void CreateMemoryPool(uint32_t size)
	{
		_memoryManager = new MemoryManager(size);
	}
	void DeleteMemoryPool()
	{
		delete _memoryManager;
	}

	struct time
	{
		uint64_t naive = 0;
		uint64_t our = 0;
	};

	//Performance Test

	template <typename T>
	void TestPerformancePoolAllocator();
	template <typename T>
	void TestPerformancePoolAllocatorThreaded();
	template <typename T>
	void ThreadPerformancePool(uint32_t nrOfObjects, std::promise<time> &p);
	template <typename T>
	void TestPerformanceStackAllocator();
	template <typename T>
	void TestPerformanceStackAllocatorThreaded();
	template <typename T>
	void ThreadPerformanceStack(uint32_t nrOfObjects, std::promise<time> &p);
	template <typename T>
	uint64_t TestRandomNewDeleteNaive();
	template <typename T>
	uint64_t TestRandomNewDeletePool(uint32_t allign);

	//"Realcase" test (T must be int for this)
	void TestWriteIntPool();
	void ThreadedWriteIntPool(uint32_t nrOfObjects, std::promise<uint32_t**> &p, uint32_t threadID);
	void TestWriteIntStack();
	void ThreadedWriteIntStack(uint32_t nrOfObjects, std::promise<uint32_t**> &p, uint32_t threadID);

	//Read and Write test for Cache coherency
	void TestRWCachePool();
	void TestRWCacheStack();
	void TestRWCacheNaive();
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


private:
	int *randomNumbers;
	int *randomsEachFrame;
	int enemiesCreated;
	Timer timer;

	MemoryManager* _memoryManager;
	

};

#pragma region Constructor & Deconstructor

TestCaseC::TestCaseC()
{
	srand(10);
	randomNumbers = new int[NR_OF_TESTS];
	randomsEachFrame = new int[NR_OF_TESTS];
	std::vector<int> usedNumbers;
	
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		randomNumbers[i] = rand() % 20 + 1;
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		randomsEachFrame[i] = i;
		
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		int random1 = rand() % NR_OF_TESTS;
		int random2 = rand() % NR_OF_TESTS;
		int temp = randomsEachFrame[random1];
		randomsEachFrame[random1] = randomsEachFrame[random2];
		randomsEachFrame[random2] = temp;
	}

	enemiesCreated = 0;

}

TestCaseC::~TestCaseC()
{

}


#pragma endregion


#pragma region TestCases

template <typename T>
void TestCaseC::TestPerformancePoolAllocator()
{

	srand(10);
	PoolAllocator* poolAllocator = _memoryManager->CreatePoolAllocator(sizeof(T), NR_OF_TESTS, 0);
	T **testCase = new T*[NR_OF_TESTS];
	uint32_t *deleteOrder = new uint32_t[NR_OF_TESTS];
	for (uint32_t i = 0; i < NR_OF_TESTS; i++)
	{
		deleteOrder[i] = i;
	}
	uint32_t swapElement1, swapElement2, temp;
	for (uint32_t i = 0; i < NR_OF_TESTS; i++)
	{
		swapElement1 = rand() % NR_OF_TESTS;
		swapElement2 = rand() % NR_OF_TESTS;

		temp = deleteOrder[swapElement1];
		deleteOrder[swapElement1] = deleteOrder[swapElement2];
		deleteOrder[swapElement2] = temp;
	}

	uint64_t testCaseNaiveTime = 0;
	uint64_t testCasePoolTime = 0;

	timer.Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		testCase[i] = new T();
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		delete testCase[deleteOrder[i]];
	}
	testCaseNaiveTime = timer.Elapsed().count();

	timer.Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		testCase[i] = (T*)poolAllocator->Malloc();
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		poolAllocator->Free((char*)testCase[deleteOrder[i]]);
	}
	testCasePoolTime = timer.Elapsed().count();
	std::cout << std::fixed << "Naive Test Case Performance single thread: " << testCaseNaiveTime << " ms" << std::endl << "Pool Test Case Performance single thread: " << testCasePoolTime << " ms" << std::endl;
	delete[] testCase;
	_memoryManager->ReleasePoolAllocator(poolAllocator);
}

template <typename T>
void TestCaseC::TestPerformancePoolAllocatorThreaded()
{

	srand(10);
	T **testCase = new T*[NR_OF_TESTS];
	

	uint64_t testCaseNaiveTime = 0;
	uint64_t testCasePoolTime = 0;

	std::promise<time> **promises = new std::promise<time>*[NR_OF_THREADS];
	std::thread **threads = new std::thread*[NR_OF_THREADS];
	testCaseNaiveTime = 0;
	testCasePoolTime = 0;
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		promises[i] = new std::promise<time>;
		threads[i] = new std::thread([this, &promises, i] {this->ThreadPerformancePool<T>(NR_OF_TESTS / NR_OF_THREADS, *promises[i]);});
	}
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		threads[i]->join();
		auto f = promises[i]->get_future();
		auto get = f.get();
		testCaseNaiveTime += get.naive;
		testCasePoolTime += get.our;
		delete threads[i];
		delete promises[i];
	}

	std::cout << std::fixed << "Naive Test Case Performance " << NR_OF_THREADS << " threads: " << ((float)testCaseNaiveTime)/NR_OF_THREADS << " ms" << std::endl << "Pool Test Case Performance " << NR_OF_THREADS << " threads: " << ((float)testCasePoolTime)/NR_OF_THREADS << " ms" << std::endl << std::endl;
	delete[] threads;
	delete[] promises;
}


template <typename T>
void TestCaseC::TestPerformanceStackAllocator()
{
	StackAllocator* stackAllocatorLock = _memoryManager->CreateStackAllocator(sizeof(T) * NR_OF_TESTS);
	T **testCase = new T*[NR_OF_TESTS];


	uint64_t testCaseNaiveTime = 0;
	uint64_t testCaseStackTime = 0;


	timer.Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		testCase[i] = new T();
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		delete testCase[i];
	}
	testCaseNaiveTime += timer.Elapsed().count();

	timer.Reset();

	stackAllocatorLock->Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		testCase[i] = (T*)stackAllocatorLock->Alloc(sizeof(T));
	}
	testCaseStackTime += timer.Elapsed().count();

	delete[] testCase;
	std::cout << std::fixed << "Naive Test Case Performance single thread: " << testCaseNaiveTime << " ms" << std::endl << "Stack Test Case Performance single thread: " << testCaseStackTime << " ms" << std::endl;
	_memoryManager->ReleaseStackAllocator(stackAllocatorLock);
}

template <typename T>
void TestCaseC::TestPerformanceStackAllocatorThreaded()
{
	uint64_t testCaseNaiveTime = 0;
	uint64_t testCaseStackTime = 0;

	std::promise<time> **promises = new std::promise<time>*[NR_OF_THREADS];
	std::thread **threads = new std::thread*[NR_OF_THREADS];
	testCaseNaiveTime = 0;
	testCaseStackTime = 0;
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		promises[i] = new std::promise<time>;
		threads[i] = new std::thread([this, &promises, i] {this->ThreadPerformanceStack<T>(NR_OF_TESTS / NR_OF_THREADS, *promises[i]);});
	}
	timer.Reset();
	
	testCaseStackTime += timer.Elapsed().count();
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		threads[i]->join();
		auto f = promises[i]->get_future();
		auto get = f.get();
		testCaseNaiveTime += get.naive;
		testCaseStackTime += get.our;
		delete threads[i];
		delete promises[i];
	}

	std::cout << std::fixed << "Naive Test Case Performance " << NR_OF_THREADS << " threads: " << ((float)testCaseNaiveTime)/NR_OF_THREADS << " ms" << std::endl << "Stack Test Case Performance " << NR_OF_THREADS << " threads: " << ((float)testCaseStackTime) / NR_OF_THREADS << " ms" << std::endl << std::endl;
	delete[] threads;
	delete[] promises;
}

template <typename T>
void TestCaseC::ThreadPerformancePool(uint32_t nrOfObjects, std::promise<time> &p)
{
	time returnTime;
	T **testCase = new T*[nrOfObjects];
	
	Timer temp(true);
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		testCase[i] = new T();
	}
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		delete testCase[i];
	}
	returnTime.naive += temp.Elapsed().count();

	temp.Reset();
	PoolAllocator* poolAllocator = _memoryManager->CreatePoolAllocator(sizeof(T), nrOfObjects, 0);
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		testCase[i] = (T*)poolAllocator->Malloc();
	}
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		poolAllocator->Free((char*)testCase[i]);
	}
	returnTime.our += temp.Elapsed().count();

	p.set_value(returnTime);
	_memoryManager->ReleasePoolAllocator(poolAllocator);
}

template <typename T>
void TestCaseC::ThreadPerformanceStack(uint32_t nrOfObjects, std::promise<time> &p)
{
	time returnTime;
	T **testCase = new T*[nrOfObjects];

	Timer temp(true);
	StackAllocator* stackAllocator = _memoryManager->CreateStackAllocator(sizeof(T) * nrOfObjects);
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{		
		testCase[i] = new T();
	}
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		delete testCase[i];
	}
	returnTime.naive = temp.Elapsed().count();
	temp.Reset();
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		testCase[i] = (T*)stackAllocator->Alloc(sizeof(T));
	}
	returnTime.our = temp.Elapsed().count();
	p.set_value(returnTime);
	_memoryManager->ReleaseStackAllocator(stackAllocator);
}

void TestCaseC::TestWriteIntPool()
{
	srand(10);
	uint32_t nrOfInts = 10;
	std::promise<uint32_t**> **promises = new std::promise<uint32_t**>*[NR_OF_THREADS];
	std::thread **threads = new std::thread*[NR_OF_THREADS];
	
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		promises[i] = new std::promise<uint32_t**>;
		threads[i] = new std::thread([this, &promises, i, nrOfInts] {this->ThreadedWriteIntPool(nrOfInts, *promises[i], i); });
	}
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		threads[i]->join();
		auto f = promises[i]->get_future();
		auto get = f.get();
		std::cout << "Thread ID: " << i << " writing out:" << std::endl;
		for (uint32_t j = 0; j < nrOfInts; j++)
		{
			std::cout << *get[j] << std::endl;
		}
		delete threads[i];
		delete promises[i];
	}

	delete[] threads;
	delete[] promises;
}
void TestCaseC::ThreadedWriteIntPool(uint32_t nrOfObjects, std::promise<uint32_t**> &p, uint32_t threadID)
{
	PoolAllocator* poolAllocator = _memoryManager->CreatePoolAllocator(sizeof(uint32_t), nrOfObjects, 0);
	uint32_t** temp = new uint32_t*[nrOfObjects];
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		temp[i] = (uint32_t*)poolAllocator->Malloc();
		*temp[i] = threadID;
	}
	p.set_value(temp);
	_memoryManager->ReleasePoolAllocator(poolAllocator);
}
void TestCaseC::TestWriteIntStack()
{
	srand(10);
	uint32_t nrOfInts = 10;
	std::promise<uint32_t**> **promises = new std::promise<uint32_t**>*[NR_OF_THREADS];
	std::thread **threads = new std::thread*[NR_OF_THREADS];

	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		promises[i] = new std::promise<uint32_t**>;
		threads[i] = new std::thread([this, &promises, i, nrOfInts] {this->ThreadedWriteIntStack(nrOfInts, *promises[i], i); });
	}
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		threads[i]->join();
		auto f = promises[i]->get_future();
		auto get = f.get();
		std::cout << "Thread ID: " << i << " writing out:" << std::endl;
		for (uint32_t j = 0; j < nrOfInts; j++)
		{
			std::cout << *get[j] << std::endl;
		}
		delete threads[i];
		delete promises[i];
	}

	delete[] threads;
	delete[] promises;
}
void TestCaseC::ThreadedWriteIntStack(uint32_t nrOfObjects, std::promise<uint32_t**> &p, uint32_t threadID)
{
	StackAllocator* stackAllocator = _memoryManager->CreateStackAllocator(sizeof(uint32_t)*nrOfObjects);
	uint32_t** temp = new uint32_t*[nrOfObjects];
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		temp[i] = (uint32_t*)stackAllocator->Alloc(sizeof(uint32_t));
		*temp[i] = threadID;
	}
	p.set_value(temp);
	_memoryManager->ReleaseStackAllocator(stackAllocator);
}


void TestCaseC::TestRWCachePool()
{
	srand(10);
	PoolAllocator* pool = _memoryManager->CreatePoolAllocator(sizeof(Enemies), NR_OF_CACHE_TESTS, 0);
	Enemies* arr[NR_OF_CACHE_TESTS];

	for (int i = 0; i < NR_OF_CACHE_TESTS; i++)
	{
		arr[i] = (Enemies*)pool->Malloc();
		arr[i]->Init();
	}
	for (int i = NR_OF_CACHE_TESTS - 1; i >= 0; i--)
	{
		arr[i]->Tick();
	}
	for (int i = 0; i < NR_OF_CACHE_TESTS; i++)
	{
		arr[i]->Tick();
	}
	for (int i = 0; i < NR_OF_CACHE_TESTS; i++)
	{
		pool->Free((char*) arr[i]);
	}
	_memoryManager->ReleasePoolAllocator(pool);

}
void TestCaseC::TestRWCacheStack()
{
	srand(10);
	StackAllocator* stack = _memoryManager->CreateStackAllocator(sizeof(Enemies)*NR_OF_CACHE_TESTS);
	Enemies* arr[NR_OF_CACHE_TESTS];

	for (int i = 0; i < NR_OF_CACHE_TESTS; i++)
	{
		arr[i] = (Enemies*)stack->AllocAligned(sizeof(Enemies), (uint32_t)__max(8, powl(2, ceil(log2l(sizeof(Enemies))))));
		arr[i]->Init();
	}
	for (int i = NR_OF_CACHE_TESTS - 1; i >= 0; i--)
	{
		arr[i]->Tick();
	}
	for (int i = 0; i < NR_OF_CACHE_TESTS; i++)
	{
		arr[i]->Tick();
	}
	_memoryManager->ReleaseStackAllocator(stack);
}
void TestCaseC::TestRWCacheNaive()
{
	srand(10);
	Enemies* arr[NR_OF_CACHE_TESTS];

	for (int i = 0; i < NR_OF_CACHE_TESTS; i++)
	{
		arr[i] = new Enemies;
		arr[i]->Init();
	}
	for (int i = NR_OF_CACHE_TESTS - 1; i >= 0; i--)
	{
		arr[i]->Tick();
	}
	for (int i = 0; i < NR_OF_CACHE_TESTS; i++)
	{
		arr[i]->Tick();
	}
	for (int i = 0; i < NR_OF_CACHE_TESTS; i++)
	{
		delete arr[i];
	}
}


template <typename T>
uint64_t TestCaseC::TestRandomNewDeleteNaive()
{
	T **testData = new T*[NR_OF_TESTS];
	timer.Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		testData[i] = new T;
	}
	for (int i = 0; i < NR_OF_TESTS/10; i++)
	{
		delete testData[randomsEachFrame[i]];
	}
	for (int i = (NR_OF_TESTS / 10)-1; i >= 0; i--)
	{
		testData[randomsEachFrame[i]] = new T;
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		delete testData[i];
	}
	delete[] testData;
	return timer.Elapsed().count();
}
template <typename T>
uint64_t TestCaseC::TestRandomNewDeletePool(uint32_t allign)
{
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
	return totalTime;
}


#pragma endregion







#endif

