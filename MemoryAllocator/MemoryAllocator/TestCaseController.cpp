#include "TestCaseController.h"
#include <iostream>

#pragma region Constructor & Deconstructor

TestCaseC::TestCaseC()
{
	srand(10);
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
	Enemies *testCase[MAX_ENEMIES_AT_ONCE];

	bool testCaseDead[MAX_ENEMIES_AT_ONCE];

	int testCaseNaiveTime = 0;
	int testCasePoolTime = 0;

	for (int i = 0; i < MAX_ENEMIES_AT_ONCE; i++)
	{
		testCaseDead[i] = false;
	}

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



	std::cout << std::fixed << testCaseNaiveTime/1000.0f << std::endl;
}

void TestCaseC::TestStackAllocator()
{

}


#pragma endregion
