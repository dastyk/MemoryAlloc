#include "TestCaseController.h"
#define NR_OF_LOOPS 100

int main()
{
	TestCaseC test;
   try
   {
	   test.CreateMemoryPool(2U * 1024U * 1024U * 1024U);
	   test.TestPerformancePoolAllocator<matrix>();
	   test.TestPerformancePoolAllocatorThreaded<matrix>();
	   test.DeleteMemoryPool();
	   test.CreateMemoryPool(2U * 1024U * 1024U * 1024U);
	   test.TestPerformanceStackAllocator<matrix>();
	   test.TestPerformanceStackAllocatorThreaded<matrix>();
	   test.DeleteMemoryPool();

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