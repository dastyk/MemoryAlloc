#include "TestCaseController.h"

int main()
{
	TestCaseC test;

   try
   {
	   //FEL PÅ NAMNEN! HINNER INTE FIXA, FIXAR IMORGON // ANDREAS
      /*test.TestPoolAllocatorThreaded<Enemies>();
      test.TestStackAllocatorThreaded<Enemies>();
      test.TestPoolAllocator<Enemies>();
      test.TestStackAllocator<Enemies>();*/
	  test.TestWriteIntStack();
   }
   catch (std::runtime_error& err)
   {
      std::cout << err.what() << std::endl;
   }

	system("pause");
	return 0;
}