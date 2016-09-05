#ifndef _TEST_CASE_CONTROLLER_H
#define _TEST_CASE_CONTROLLER_H


#include "TestStructs.h"
#include <iostream>
#include <random>
#include "Timer.h"

#define NR_OF_TESTS 50000
#define MAX_EACH_FRAME 10
#define MAX_ENEMIES_AT_ONCE NR_OF_TESTS*MAX_EACH_FRAME

class TestCaseC
{
public:
	TestCaseC();
	~TestCaseC();

	void TestPoolAllocator();
	void TestStackAllocator();



private:
	int *randomNumbers;
	int *randomsEachFrame;
	int enemiesCreated;
	Timer timer;
	//Insert allocators here
};










#endif

