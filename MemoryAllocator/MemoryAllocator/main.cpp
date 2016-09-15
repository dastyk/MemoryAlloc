#include "TestCaseController.h"

int main()
{
	//TestCaseC test;
 //  try
 //  {
	//   test.CreateMemoryPool(2U * 1024U * 1024U * 1024U);
	//   for (int i = 0; i < 1000; i++)
	//   {
	//	   switch (1)
	//	   {
	//	   case 1:
	//		   test.TestRWCacheNaive();
	//		   break;

	//	   case 2:
	//		   test.TestRWCachePool();
	//		   break;

	//	   case 3:
	//		   test.TestRWCacheStack();
	//		   break;
	//	   }
	//   }
	//   test.DeleteMemoryPool();
 //  }
 //  catch (std::runtime_error& err)
 //  {
 //     std::cout << err.what() << std::endl;
 //  }

	try
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
//		StackAllocator* sa7 = m.CreateStackAllocator(1319-sizeof(StackAllocator));
	
	//	m.PrintBlockInfo();
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
//		m.ReleaseStackAllocator(sa7);
		m.ReleasePoolAllocator(pa1);
		
		m.PrintBlockInfo();
		std::cout << std::endl;
		

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
	return 0;
}