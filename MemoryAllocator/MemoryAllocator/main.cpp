#include "TestCaseController.h"
int main()
{
	TestCaseC test;

	/*std::cout << "Pool Allocator: " << std::endl;
	test.TestPerformancePoolAllocatorThreaded<Enemies>();
	std::cout << "Stack Allocator: " << std::endl;
	test.TestPerformanceStackAllocatorThreaded<Enemies>();*/

	test.TestWriteIntStack();

	system("pause");
	return 0;
}