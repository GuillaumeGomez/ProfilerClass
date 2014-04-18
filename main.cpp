#include <iostream>
#include "Profiler.hpp"

#ifdef WIN32
#include "windows.h"
#else
#include <unistd.h>
#endif

void  wait_for_it(int wait_time)
{
  PROFILER_CALL
#ifdef WIN32
  Sleep(wait_time);
#else
  usleep(wait_time * 1000);
#endif
}

void  why()
{
  PROFILER_CALL
  std::cout << "in why !" << std::endl;
}

int main()
{
  //PROFILER_OUTPUT_FILE("results.txt")
  PROFILER_CALL
  std::cout << "Hello World !" << std::endl;
  wait_for_it(500);
  why();
  wait_for_it(500);
  return 0;
}

