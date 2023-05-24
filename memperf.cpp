#include <chrono>
#include <cstring>
#include <iostream>
#include <vector>

#define TEST_MALLOC 0
#define TEST_MEMSET 0
#define TEST_RESIZE 1

enum class test_type
{
  malloc,
  memset,
  resize
};

// single test run
void do_run(uint64_t runs, test_type ttype, int size /*Bytes*/)
{
  // log parameter
  std::cout << "--------------------------------------------"     << std::endl;
  std::cout << "Runs                    : " << runs               << std::endl;
  std::cout << "Message size            : " << size/1024 << " kB" << std::endl;

  // get start time stamp
  auto start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  volatile uint64_t run(0);
  switch(ttype)
  {
    case test_type::malloc:
    {
      std::cout << "Test Type               : MALLOC" << std::endl;
      for (; run < runs; ++run) {
        auto ptr = std::malloc(size);
        std::free(ptr);
      }
    }
      break;
    case test_type::memset:
    {
      std::cout << "Test Type               : MEMSET" << std::endl;
      std::vector<char> m_buffer(size);
      for (; run < runs; ++run) {
        memset(m_buffer.data(), 0, m_buffer.size());
      }
    }
      break;
    case test_type::resize:
    {
      std::cout << "Test Type               : RESIZE" << std::endl;
      std::vector<char> m_buffer(size);
      for (; run < runs; ++run) {
        m_buffer.clear();
        m_buffer.resize(size);
      }
    }
      break;
  }

  // get microseconds
  auto end_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  auto diff_time = end_time - start_time;

  // log test
  std::cout << "SumTime                 : " << diff_time                                                    << " ms" << std::endl;
  std::cout << "Time/Action             : " << static_cast<double>(diff_time) / static_cast<double>(runs)   << " ms" << std::endl;
  std::cout << "Actions/Second          : " << static_cast<double>(runs) / (static_cast<double>(diff_time) / 1000.0) << std::endl;
  std::cout << "MB/Second               : " << static_cast<double>(runs * size) / (1024.0 * 1024.0) / (static_cast<double>(diff_time) / 1000) << std::endl;
  std::cout << "--------------------------------------------"                                                        << std::endl;
}

int main()
{
  const int runs(10000);
  const int size(2 * 1024 * 1024);

  do_run(runs, test_type::malloc, size);
  do_run(runs, test_type::memset, size);
  do_run(runs, test_type::resize, size);

  return(0);
}
