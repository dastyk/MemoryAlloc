#include "TestCaseController.h"

int main()
{
	system("pause");
	TestCaseC test;

   try
   {
	  
      test.TestPerformancePoolAllocatorThreaded<Enemies>();
      test.TestPerformanceStackAllocatorThreaded<Enemies>();
	  test.TestWriteIntStack();
   }
   catch (std::runtime_error& err)
   {
      std::cout << err.what() << std::endl;
   }

	system("pause");
	return 0;
}