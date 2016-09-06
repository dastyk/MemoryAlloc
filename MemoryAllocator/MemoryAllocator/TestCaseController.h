#ifndef _TEST_CASE_CONTROLLER_H
#define _TEST_CASE_CONTROLLER_H


#include "TestStructs.h"
#include <iostream>
#include <random>
#include <thread>
#include <future>
#include "Timer.h"


#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "PoolAllocatorLock.h"
#include "StackAllocatorLock.h"


#define NR_OF_TESTS 5000000
#define NR_OF_THREADS 8

class TestCaseC
{
public:
	TestCaseC();
	~TestCaseC();
	struct time
	{
		__int64 naive = 0;
		__int64 our = 0;
	};

	template <typename T>
	void TestPoolAllocator();
	template <typename T>
	void TestPoolAllocatorThreaded();
	template <typename T>
	void TestStackAllocator();
	template <typename T>
	void TestStackAllocatorThreaded();
	template <typename T>
	void ThreadPool(uint32_t nrOfObjects, std::promise<time> &p);
	template <typename T>
	void ThreadStack(uint32_t nrOfObjects, std::promise<time> &p);



private:
	int *randomNumbers;
	int *randomsEachFrame;
	int enemiesCreated;
	Timer timer;

	PoolAllocator* poolAllocator;
	PoolAllocatorLock* poolAllocatorLock;
	StackAllocator* stackAllocator;
	StackAllocatorLock* stackAllocatorLock;


};

#pragma region Constructor & Deconstructor

TestCaseC::TestCaseC()
{
	srand(10);
	randomNumbers = new int[NR_OF_TESTS];
	randomsEachFrame = new int[NR_OF_TESTS];
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		randomNumbers[i] = rand() % 20 + 1;
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		randomsEachFrame[i] = rand() % NR_OF_TESTS + 1;
	}

	enemiesCreated = 0;

}

TestCaseC::~TestCaseC()
{

}


#pragma endregion


#pragma region TestCases

template <typename T>
void TestCaseC::TestPoolAllocator()
{
	srand(10);
	poolAllocator = new PoolAllocator(sizeof(T), NR_OF_TESTS);
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

	__int64 testCaseNaiveTime = 0;
	__int64 testCasePoolTime = 0;

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
		poolAllocator->Free(testCase[deleteOrder[i]]);
	}
	testCasePoolTime = timer.Elapsed().count();
	std::cout << std::fixed << "Naive Test Case Performance: " << testCaseNaiveTime << " ms" << std::endl << "Pool Test Case Performance: " << testCasePoolTime << " ms" << std::endl << std::endl;
	delete[] testCase;
}

template <typename T>
void TestCaseC::TestPoolAllocatorThreaded()
{
	srand(10);
	poolAllocatorLock = new PoolAllocatorLock(sizeof(T), NR_OF_TESTS);
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

	__int64 testCaseNaiveTime = 0;
	__int64 testCasePoolTime = 0;

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
		testCase[i] = (T*)poolAllocatorLock->Malloc();
	}
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		poolAllocatorLock->Free(testCase[deleteOrder[i]]);
	}
	testCasePoolTime = timer.Elapsed().count();
	std::cout << std::fixed << "Naive Test Case Performance single thread: " << testCaseNaiveTime << " ms" << std::endl << "Pool Test Case Performance single thread: " << testCasePoolTime << " ms" << std::endl;
	delete[] testCase;

	std::promise<time> **promises = new std::promise<time>*[NR_OF_THREADS];
	std::thread **threads = new std::thread*[NR_OF_THREADS];
	testCaseNaiveTime = 0;
	testCasePoolTime = 0;
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		promises[i] = new std::promise<time>;
		threads[i] = new std::thread([this, &promises, i] {this->ThreadPool<T>(NR_OF_TESTS / NR_OF_THREADS, *promises[i]);});
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
void TestCaseC::TestStackAllocator()
{
	stackAllocator = new StackAllocator(sizeof(T)*NR_OF_TESTS);
	T **testCase = new T*[NR_OF_TESTS];


	__int64 testCaseNaiveTime = 0;
	__int64 testCasePoolTime = 0;


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

	stackAllocator->Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		testCase[i] = (T*)stackAllocator->Alloc(sizeof(T));
	}
	testCasePoolTime += timer.Elapsed().count();

	delete[] testCase;
	std::cout << std::fixed << "Naive Test Case Performance: " << testCaseNaiveTime << " ms" << std::endl << "Stack Test Case Performance: " << testCasePoolTime << " ms" << std::endl << std::endl;
}

template <typename T>
void TestCaseC::TestStackAllocatorThreaded()
{
	stackAllocatorLock = new StackAllocatorLock(sizeof(T)*NR_OF_TESTS);
	T **testCase = new T*[NR_OF_TESTS];


	__int64 testCaseNaiveTime = 0;
	__int64 testCaseStackTime = 0;


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

	std::promise<time> **promises = new std::promise<time>*[NR_OF_THREADS];
	std::thread **threads = new std::thread*[NR_OF_THREADS];
	testCaseNaiveTime = 0;
	testCaseStackTime = 0;
	stackAllocatorLock->Reset();
	for (uint32_t i = 0; i < NR_OF_THREADS; i++)
	{
		promises[i] = new std::promise<time>;
		threads[i] = new std::thread([this, &promises, i] {this->ThreadStack<T>(NR_OF_TESTS / NR_OF_THREADS, *promises[i]);});
	}
	timer.Reset();
	stackAllocatorLock->Reset();
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
void TestCaseC::ThreadPool(uint32_t nrOfObjects, std::promise<time> &p)
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
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		testCase[i] = (T*)poolAllocatorLock->Malloc();
	}
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		poolAllocatorLock->Free(testCase[i]);
	}
	returnTime.our += temp.Elapsed().count();

	p.set_value(returnTime);
}

template <typename T>
void TestCaseC::ThreadStack(uint32_t nrOfObjects, std::promise<time> &p)
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
	returnTime.naive = temp.Elapsed().count();
	temp.Reset();
	for (uint32_t i = 0; i < nrOfObjects; i++)
	{
		testCase[i] = (T*)stackAllocatorLock->Alloc(sizeof(T));
	}
	returnTime.our = temp.Elapsed().count();
	p.set_value(returnTime);
}

#pragma endregion







#endif

