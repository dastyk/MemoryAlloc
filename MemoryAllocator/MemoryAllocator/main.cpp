#include "TestCaseController.h"

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