
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#ifdef HAVE_SDS
#include "sds.h"
#endif
#ifdef _MSC_VER
#include "windows.h"
#endif

//local
#include "table_str.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//clock_gettime_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

class clock_gettime_t
{
public:
  clock_gettime_t();
  void start();
  void stop();
  void now(const std::string str);
private:
  timespec begin;
  timespec last;
  std::ofstream ofs;
  int running;
  int get_clock_gettime(struct timespec *spec);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//main
/////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
  clock_gettime_t timer;
  const size_t nbr = 1000 * 1000 * 10;
  const char* s = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
  size_t len = strlen(s);

  std::cout << "basic table use:" << std::endl;

  {
    table_str_t table(20);
    table.add("ab", 2);
    table.dump();
    table.add("cdefgh", 6);
    table.dump();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //sds
  /////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef HAVE_SDS
  {

    std::cout << "SDS benchmark" << std::endl;

    timer.start();
    sds sd = sdsempty();
    for (size_t idx = 0; idx < nbr; ++idx)
    {
      sd = sdscat(sd, s);
    }
    timer.now("end sd");
    sdsfree(sd);
    timer.stop();
  }
#endif

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //string
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  std::cout << "std::string benchmark append string of size " <<
    len << ", " << nbr << " times" << std::endl;

  {
    timer.start();
    std::string str;
    for (size_t idx = 0; idx < nbr; ++idx)
    {
      str += s;
    }
    timer.now("end str");
    timer.stop();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //string reserve
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  std::cout << "std::string reserve benchmark append string of size " <<
    len << ", " << nbr << " times" << std::endl;

  {
    timer.start();
    std::string str;
    str.reserve(nbr * len);
    for (size_t idx = 0; idx < nbr; ++idx)
    {
      str.append(s, len);
    }
    timer.now("end str reserve");
    timer.stop();
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //table_str_t
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  std::cout << "table string benchmark with pre-allocation of " <<
    nbr *len << " elements" << std::endl;

  {
    timer.start();
    table_str_t table(nbr * len);
    for (size_t idx = 0; idx < nbr; ++idx)
    {
      table.add(s, len);
    }
    timer.now("end table");
    timer.stop();
  }


  size_t nbr_alloc_factor = 100;
  std::cout << "table string benchmark with pre-allocation of ONLY " <<
    nbr * len / nbr_alloc_factor << " elements, allocation is MADE " <<
    nbr_alloc_factor << " times...patience..."  << std::endl;

  {
    timer.start();
    table_str_t table(nbr * len / nbr_alloc_factor);
    for (size_t idx = 0; idx < nbr; ++idx)
    {
      table.add(s, len);
    }
    timer.now("end table");
    timer.stop();
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//clock_gettime_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

clock_gettime_t::clock_gettime_t() :
  running(0)
{
  ofs.open("timer.txt", std::ofstream::binary);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//clock_gettime_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

void clock_gettime_t::start()
{
  if (!running)
  {
    get_clock_gettime(&begin);
    get_clock_gettime(&last);
    running = 1;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//clock_gettime_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

void clock_gettime_t::stop()
{
  running = 0;
  get_clock_gettime(&begin);
  get_clock_gettime(&last);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//clock_gettime_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

void clock_gettime_t::now(const std::string str)
{
  timespec tnow;
  get_clock_gettime(&tnow);
  double time_total = (double)(tnow.tv_sec - begin.tv_sec);
  double time_last = (double)(tnow.tv_sec - last.tv_sec);
  std::cout << std::setprecision(1) << std::fixed;
  ofs << std::setprecision(1) << std::fixed;
  ofs << str << ":\t" << time_last << " seconds\t" << time_total << " total" << std::endl;
#ifdef HAVE_GUI
  wxLogDebug("%s\t%f seconds\t%f total", str.c_str(), time_last, time_total);
#else
  std::cout << str << ":\t" << time_last << " seconds\t" << time_total << " total" << std::endl;
#endif
  last = tnow;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//clock_gettime_t
/////////////////////////////////////////////////////////////////////////////////////////////////////

int clock_gettime_t::get_clock_gettime(struct timespec *spec)
{
#ifdef _MSC_VER
  __int64 wintime;
  GetSystemTimeAsFileTime((FILETIME*)&wintime);
  wintime -= 116444736000000000i64; //1jan1601 to 1jan1970
  spec->tv_sec = wintime / 10000000i64; //seconds
  spec->tv_nsec = wintime % 10000000i64 * 100; //nano-seconds
#else
  clock_gettime(CLOCK_MONOTONIC, spec);
#endif
  return 0;
}



