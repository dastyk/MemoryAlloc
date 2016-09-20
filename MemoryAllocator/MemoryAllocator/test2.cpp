#include "MemoryManager.h"
#include "Timer.h"
#include <math.h>
#include <iostream>
#include <string>
int main()
{

  MemoryManager m(2U*1024U*1024U*1024U);
  Timer t;
  uint32_t num = 1000000;
  static const int s = 10;
  uint32_t times[s];
  for(int i = 0; i< s; i++)
  {
    times[i] = 0;
  }
  for(uint32_t k = 0; k < 100; k++)
  {
  for(uint8_t i = 0; i < s; i++)
  {
    uint8_t size = 4*pow(2,i);
  //  char* mem = new char[size*num];
    PoolAllocator* p = m.CreatePoolAllocator(size, num, 16);
    t.Reset();
    for(uint32_t j = 0; j < num; j++)
    {
      p->Malloc();
    }
    times[i] += t.Elapsed().count();
    m.ReleasePoolAllocator(p);
  }

}

for(uint8_t i = 0; i < s; i++)
{
  std::cout << times[i]/100 << std::endl;
}


  return 0;
}
