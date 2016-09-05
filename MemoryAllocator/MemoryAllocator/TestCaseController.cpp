#include "TestCaseController.h"
#include <iostream>
#include "PoolAllocator.h"

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
		randomsEachFrame[i] = rand() % MAX_EACH_FRAME + 1;
	}

	enemiesCreated = 0;
	
}

TestCaseC::~TestCaseC()
{

}


#pragma endregion


#pragma region TestCases

void TestCaseC::TestPoolAllocator()
{
	PoolAllocator a(12, MAX_ENEMIES_AT_ONCE);
	Enemies **testCase = new Enemies*[MAX_ENEMIES_AT_ONCE];

	bool *testCaseDead = new bool[MAX_ENEMIES_AT_ONCE];

	int testCaseNaiveTime = 0;
	int testCasePoolTime = 0;

	for (int i = 0; i < MAX_ENEMIES_AT_ONCE; i++)
	{
		testCaseDead[i] = false;
	}
	timer.Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		for (int j = 0; j < randomsEachFrame[i]; j++)
		{
			testCase[enemiesCreated] = new Enemies();
			testCase[enemiesCreated]->Init();
			enemiesCreated++;
		}

		for (int j = 0; j < enemiesCreated; j++)
		{
			if (!testCaseDead[j])
			{
				testCase[j]->Tick();
				if (!testCase[j]->Alive())
				{
					delete testCase[j];
					testCaseDead[j] = true;
				}
			}
		}

	}

	for (int j = 0; j < enemiesCreated; j++)
	{
		if (!testCaseDead[j])
		{
			delete testCase[j];
			testCaseDead[j] = true;
		}
	}
	testCaseNaiveTime += timer.Elapsed().count();

	enemiesCreated = 0;
	for (int i = 0; i < MAX_ENEMIES_AT_ONCE; i++)
	{
		testCaseDead[i] = false;
	}
	timer.Reset();
	for (int i = 0; i < NR_OF_TESTS; i++)
	{
		for (int j = 0; j < randomsEachFrame[i]; j++)
		{
			testCase[enemiesCreated] = (Enemies*)a.Malloc();
			testCase[enemiesCreated]->Init();
			enemiesCreated++;
		}

		for (int j = 0; j < enemiesCreated; j++)
		{
			if (!testCaseDead[j])
			{
				testCase[j]->Tick();
				if (!testCase[j]->Alive())
				{
					a.Free(testCase[j]);
					testCaseDead[j] = true;
				}
			}
		}

	}

	for (int j = 0; j < enemiesCreated; j++)
	{
		if (!testCaseDead[j])
		{
			a.Free(testCase[j]);
			testCaseDead[j] = true;
		}
	}
	testCasePoolTime += timer.Elapsed().count();

	std::cout << std::fixed << testCaseNaiveTime << "ms" <<  std::endl << testCasePoolTime << "ms" << std::endl;
}

void TestCaseC::TestStackAllocator()
{

}


#pragma endregion
