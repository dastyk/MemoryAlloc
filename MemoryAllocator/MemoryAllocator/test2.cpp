#include "MemoryManager.h"
#include "Timer.h"
#include <math.h>
#include <iostream>
#include <string.h>
int main()
{

  //MemoryManager m(2U*1024U*1024U*1024U);
  Timer t;
  uint32_t num = 100;
  static const int s = 23;
  uint32_t times[s];
  for(int i = 0; i< s; i++)
  {
    times[i] = 0;
  }
  uint8_t i = 0;
  for(uint32_t k = 0; k < 10; k++)
  {
  for( i = 0; i < s; i++)
  {
    uint64_t size = 4*pow(2,i);
    void** slots = new void*[num];
    //  char* mem = new char[size*num];
    //PoolAllocator* p = m.CreatePoolAllocator(size, num, 0);

    std::cout << "Size: " << size << " B, Objects: " << num << std::endl;
    t.Reset();
    for(uint32_t j = 0; j < num; j++)
    {
      slots[j] = operator new(size);
     memset(slots[j], 0, size);
      //p->Malloc();
    }
    times[i] += t.Elapsed().count();
    for(uint32_t j = 0; j < num; j++)
    {
      operator delete(slots[j]);
      //p->Malloc();
    }
    delete[] slots;
    //m.ReleasePoolAllocator(p);

  }

}
uint8_t n = i;
for(uint8_t i = 0; i < n; i++)
{
  uint64_t size = 4*pow(2,i);
  std::cout << "Size: " << size << " B, Times: ";
  std::cout << times[i]/100000 << std::endl;
}


  return 0;
}
