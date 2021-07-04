
#include <sched.h>
#include <sys/mman.h>
#include <iostream>

#include "Application.hpp"

void configure_realtime()
{
    const int realtime = 3;
  {
    cpu_set_t cpuset = {};
    CPU_ZERO(&cpuset);
    CPU_SET(realtime, &cpuset);

    const int r = ::sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    if (r < 0)
    {
        throw std::runtime_error("Error setting CPU affinity");
    }

    std::cout << "Affinity set to " << realtime << "\n";
  }

  {
    struct sched_param params = {};
    params.sched_priority = 10;
    const int r = ::sched_setscheduler(0, SCHED_RR, &params);
    if (r < 0)
    {
      throw std::runtime_error("Error setting realtime scheduler");
    }
  }

  {
    const int r = ::mlockall(MCL_CURRENT | MCL_FUTURE);
    if (r < 0)
    {
        throw std::runtime_error("Error locking memory");
    }
  }
}

int main(int argc, char *argv[])
{
   configure_realtime();
 
   Application app;

    if (app.init())
    {
        app.run();
    }
    
    return 0;
}
