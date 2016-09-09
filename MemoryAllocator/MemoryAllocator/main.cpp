#include "TestCaseController.h"

int main()
{
	TestCaseC test;

   try
   {
	   test.TestPerformanceStackAllocatorThreaded<Enemies>();
   }
   catch (std::runtime_error& err)
   {
      std::cout << err.what() << std::endl;
   }

	system("pause");
	return 0;
}