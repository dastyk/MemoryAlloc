#include "TestCaseController.h"

int main()
{
	TestCaseC test;

   try
   {
      test.TestPoolAllocatorThreaded<Enemies>();
      test.TestStackAllocatorThreaded<Enemies>();
      test.TestPoolAllocator<Enemies>();
      test.TestStackAllocator<Enemies>();
   }
   catch (std::runtime_error& err)
   {
      std::cout << err.what() << std::endl;
   }

	system("pause");
	return 0;
}