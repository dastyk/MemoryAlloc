#include "TestCaseController.h"
#define NR_OF_LOOPS 100

int main()
{
	TestCaseC test;
   try
   {
	   test.CreateMemoryPool(2U * 1024U * 1024U * 1024U);
	   std::cout << "Size: " << sizeof(matrix2) << std::endl;
	   uint64_t timer1 = 0, timer2 = 0;
	   for (int i = 0; i < NR_OF_LOOPS; i++)
	   {
		   test.RandomizeTheRandom();
		   timer1 += test.TestRandomNewDeleteNaive<matrix2>();
		   timer2 += test.TestRandomNewDeletePool<matrix2>(0);
	   }
	   std::cout << "OS Method: " << 1.0f*timer1 / NR_OF_LOOPS << std::endl << "Pool method: " << 1.0f*timer2 / NR_OF_LOOPS << std::endl;

   }
   catch (std::runtime_error& err)
   {
      std::cout << err.what() << std::endl;
   }

	/*try
	{
		MemoryManager m(sizeof(int) * 16000);
		PoolAllocator* pa1 = m.CreatePoolAllocator(sizeof(int), 10, 0);
		m.PrintBlockInfo();
		std::cout << std::endl;
		StackAllocator* sa1 = m.CreateStackAllocator(3000);
		m.PrintBlockInfo();
		std::cout << std::endl;
		StackAllocator* sa2 = m.CreateStackAllocator(3000);
		m.PrintBlockInfo();
		std::cout << std::endl;
		StackAllocator* sa3 = m.CreateStackAllocator(3000);
		StackAllocator* sa4 = m.CreateStackAllocator(3000);
		StackAllocator* sa5 = m.CreateStackAllocator(3000);
		StackAllocator* sa6 = m.CreateStackAllocator(3000);

		std::cout << std::endl;
		m.ReleaseStackAllocator(sa2);
		m.PrintBlockInfo();
		std::cout << std::endl;
		m.ReleaseStackAllocator(sa1);
		m.PrintBlockInfo();
		std::cout << std::endl;
		m.ReleaseStackAllocator(sa4);
		m.PrintBlockInfo();
		std::cout << std::endl;
		m.ReleaseStackAllocator(sa3);
		m.PrintBlockInfo();
		std::cout << std::endl;
		m.ReleaseStackAllocator(sa5);
		m.ReleaseStackAllocator(sa6);
		m.PrintBlockInfo();
		std::cout << std::endl;
		m.ReleasePoolAllocator(pa1);
		
		m.PrintBlockInfo();
		std::cout << std::endl;
		

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}*/
	system("pause");
	return 0;
}