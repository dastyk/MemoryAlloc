#include "TestCaseController.h"

#include "Timer.h"
#include "PoolAllocator.h"
#include <iostream>
#include "TestStructs.h"
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
int main()
{
	//srand(1337);
	//PoolAllocator m(sizeof(Enemies), 200000);
	//std::array<uint32_t, 200000> foo;
	//for (uint32_t i = 0; i < 200000; i++)
	//{
	//	foo[i] = i;
	//}
	//// obtain a time-based seed:
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	//shuffle(foo.begin(), foo.end(), std::default_random_engine(seed));

	//Timer t1;

	//Enemies** arr = new Enemies*[200000];
	//Enemies** arr2 = new Enemies*[200000];
	//t1.Reset();
	//for (uint32_t i = 0; i < 200000; i++)
	//{
	//	arr[i] = new Enemies;

	//}
	//std::cout << "N: " << t1.Elapsed().count() << std::endl;

	//t1.Reset();
	//for (uint32_t i = 0; i < 200000; i++)
	//{
	//	arr2[i] = (Enemies*)m.Malloc();
	//}
	//std::cout << "N: " << t1.Elapsed().count() << std::endl;

	//t1.Reset();
	//for (uint32_t i = 0; i < 200000; i ++)
	//{
	//	delete arr[foo[i]];
	//}
	//std::cout << "N: " << t1.Elapsed().count() << std::endl;

	//t1.Reset();
	//for (uint32_t i = 0; i < 200000; i++)
	//{
	//	m.Free(arr2[foo[i]]);
	//}
	//std::cout << "N: " << t1.Elapsed().count() << std::endl;




	TestCaseC test;

	test.TestPoolAllocator();


	
	system("pause");
	return 0;
}