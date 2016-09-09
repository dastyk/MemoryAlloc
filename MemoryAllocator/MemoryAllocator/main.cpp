#include "TestCaseController.h"

int main()
{
	TestCaseC test;
   try
   {
	   test.CreateMemoryPool(2U * 1024U * 1024U * 1024U);
	   for (int i = 0; i < 1000; i++)
	   {
		   switch (1)
		   {
		   case 1:
			   test.TestRWCacheNaive();
			   break;

		   case 2:
			   test.TestRWCachePool();
			   break;

		   case 3:
			   test.TestRWCacheStack();
			   break;
		   }
	   }
	   test.DeleteMemoryPool();
   }
   catch (std::runtime_error& err)
   {
      std::cout << err.what() << std::endl;
   }

	return 0;
}