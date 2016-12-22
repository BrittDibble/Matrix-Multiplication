#include <boost/thread.hpp>
#include <iostream>

int main()
{
  unsigned int nthreads = boost::thread::hardware_concurrency();
  std::cout << nthreads;
}
