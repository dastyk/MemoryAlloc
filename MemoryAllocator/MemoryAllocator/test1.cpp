#include "MemoryManager.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <string.h>
#include <random>
#include <time.h>
#include "Timer.h"
#include <math.h>

struct Matrix
{
  uint32_t m[16];
  static Matrix Zero()
  {
    Matrix out;
    memset(out.m, 0, sizeof(uint32_t) * 16);
    return out;
  }
  static Matrix Rand()
  {
    Matrix out;
    for(int i = 0; i < 16; ++i)
    {
      out.m[i] = rand()%10000;
    }
  }
  uint32_t SumRow()
  {
    uint32_t sum = 0;
    for(int i = 0; i < 16; ++i)
    {
      sum += m[i];
    }
    return sum;
  }

  uint32_t SumCol()
  {
    uint32_t sum = 0;
    for(int index = 4; index < 4; index += 4)
    {
      for(int i = 0; i < 4; i++)
      {
        sum += m[index * i];
      }
    }
  }
  Matrix& operator+=(const Matrix& other)
  {
    for(uint8_t i = 0; i < 16; i++)
    {
      this->m[i] += other.m[i];
    }
    return *this;
  }
  Matrix operator+(const Matrix& other)
  {
    Matrix out;
    for(uint8_t i = 0; i < 16; i++)
    {
      out.m[i] = this->m[i] + other.m[i];
    }
    return out;
  }
  void Print()
  {
    for(uint8_t i = 0; i<16; i++)
    {
      if(i % 4 == 0)
        std::cout << "\n";
      std::cout << m[i] << " ";

    }
  }
};

struct TS
{
  Matrix a;
  Matrix b;
};

int main(int argc, char* argv[])
{
  srand(1337);

  MemoryManager m(2U*1024U*1024U*1024U);

  std::cout << "Alignment: " << argv[1] << std::endl;

  Timer t;


  static const uint8_t tests = 6;

 StackAllocator* s = m.CreateStackAllocator(12*pow(10, tests));
 Matrix tot = Matrix::Zero();
  uint32_t times[tests];
  memset(times, 0, 4*10);
  for(uint32_t l = 0; l < 100; l++)
  {
    std::cout << l << std::endl;
    for(uint32_t k = 0; k < tests; k++)
    {
      uint32_t size = 10*pow(10, k);
      PoolAllocator* p = m.CreatePoolAllocator(sizeof(TS), size*2, std::stoi(argv[1]));
      TS** ts = (TS**)s->AllocAligned(size*sizeof(TS*), 8);// new TS*[size];


      for(uint32_t i = 0; i < size; i++)
      {
        ts[i] = (TS*)p->Malloc();
        ts[i]->a = Matrix::Rand();
        ts[i]->b = Matrix::Rand();
      }



      t.Reset();
      for(uint32_t i = 0; i < size; i++)
      {
        tot += ts[i]->a + ts[i]->b;
      }

      times[k] += t.Elapsed().count();
      s->Reset();
      m.ReleasePoolAllocator(p);
    }
  }
std::cout << std::endl;
  for(uint32_t k = 0; k < tests; k++)
  {
    std::cout << times[k]/tests << std::endl;
  }

  return 0;
}
