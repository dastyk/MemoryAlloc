#include "TestCaseController.h"
#define NR_OF_LOOPS 100

int main()
{
	TestCaseC test;
   try
   {
	   test.CreateMemoryPool(10U * 1024U * 1024U * 1024U);
	   uint64_t timer1 = 0;
	   uint64_t timer2 = 0;
	   std::cout << "Testing with uint64_t of size: " << sizeof(uint64_t) << std::endl;
	   for (int i = 0; i < NR_OF_LOOPS; i++)
	   {
		   test.RandomizeTheRandom();
		   timer1 += test.TestRandomNewDeleteNaive<uint32_t>();
		   timer2 += test.TestRandomNewDeletePool<uint32_t>(0);
	   }
	   
	   std::cout << "Test Case: Random new and delete. Testing: Naive Solution." << std::endl << "Time for " << NR_OF_TESTS << " objects, 10% deleted and recreated: " << std::endl;
	   std::cout << timer1 / NR_OF_LOOPS << std::endl;

	   

	   std::cout << "Test Case: Random new and delete. Testing: Pool Solution." << std::endl << "Time for " << NR_OF_TESTS << " objects, 10% deleted and recreated: " << std::endl;
	   std::cout << timer2 / NR_OF_LOOPS << std::endl;


	   std::cout << std::endl << "Testing with a matrix of size " << sizeof(matrix) << std::endl;
	   timer1 = 0;
	   timer2 = 0;
	   for (int i = 0; i < NR_OF_LOOPS; i++)
	   {
		   test.RandomizeTheRandom();
		   timer1 += test.TestRandomNewDeleteNaive<matrix>();
		   timer2 += test.TestRandomNewDeletePool<matrix>(0);
	   }

	   std::cout << "Test Case: Random new and delete. Testing: Naive Solution." << std::endl << "Time for " << NR_OF_TESTS << " objects, 10% deleted and recreated: " << std::endl;
	   std::cout << timer1 / NR_OF_LOOPS << std::endl;



	   std::cout << "Test Case: Random new and delete. Testing: Pool Solution." << std::endl << "Time for " << NR_OF_TESTS << " objects, 10% deleted and recreated: " << std::endl;
	   std::cout << timer2 / NR_OF_LOOPS << std::endl;

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