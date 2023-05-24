#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#define TEST_MALLOC 0
#define TEST_MEMSET 0
#define TEST_RESIZE 1

// single test run
void do_run(uint64_t runs, int size /*kB*/)
{
  // log parameter
  std::cout << "--------------------------------------------" << std::endl;
  std::cout << "Runs                    : " << runs << std::endl;
  std::cout << "Message size            : " << size << " kB" << std::endl;

  // get start time stamp
  auto start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  //std::vector<int> m_buffer;
  std::vector<char> m_buffer(size * 1024);

  volatile uint64_t run(0);
  for (; run < runs; ++run)
  {
#if TEST_MALLOC
    auto ptr = std::malloc(size * 1024);
    std::free(ptr);
#endif

#if TEST_MEMSET
    memset(m_buffer.data(), 0, m_buffer.size());
#endif

#if TEST_RESIZE
    m_buffer.clear();
    m_buffer.resize(size * 1024);
#endif
  }

  // get microseconds
  auto end_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  auto diff_time = end_time - start_time;

  // log test
  std::cout << "SumTime                 : " << diff_time << " ms" << std::endl;
  std::cout << "Time/Copy               : " << static_cast<double>(diff_time) / run << " ms" << std::endl;
  std::cout << "Copies/Second           : " << runs / (static_cast<double>(diff_time) / 1000) << std::endl;
  std::cout << "MB/Second               : " << (runs * size) / 1024 / (static_cast<double>(diff_time) / 1000) << std::endl;
  std::cout << "--------------------------------------------" << std::endl;
}

int main()
{
  do_run(10000, 2048);
  return(0);
}
