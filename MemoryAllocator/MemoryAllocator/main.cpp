#include "TestCaseController.h"

int main()
{
	TestCaseC test;
	system("pause");
   try
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
   catch (std::runtime_error& err)
   {
      std::cout << err.what() << std::endl;
   }

	return 0;
}