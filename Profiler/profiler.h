#ifndef TOYSTL_PROFILER_PROFILER_H_
#define TOYSTL_PROFILER_PROFILER_H_

#include <sys/resource.h>
#include <sys/time.h>

#include <chrono>
#include <ctime>
#include <iostream>

namespace toystl {
namespace profiler {
class ProfilerInstance {
 public:
  // using SteadyClock = typename std::chrono::steady_clock;
  // using TimePoint = typename SteadyClock::time_point;
  // using DurationTime = typename std::chrono::duration<double, std::ratio<1,
  // 1>>;
  typedef typename std::chrono::steady_clock SteadyClock;
  typedef typename SteadyClock::time_point TimePoint;
  typedef typename std::chrono::duration<double, std::ratio<1, 1>> DurationTime;
  enum class MemoryUnit { KB_, MB_, GB_ };

 private:
#define KB / 1024
#define MB KB / 1024
#define GB MB / 1024

 private:
  static DurationTime duringTime_;
  static TimePoint startTime_;
  static TimePoint finishTime_;

 public:
  static void start();                                       // 开始时间
  static void end();                                         // 结束时间
  static void dumpDuringTime(std::ostream& os = std::cout);  // 持续时间

  static double second();       // 以秒为单位返回
  static double milliSecond();  // 以毫秒为单位返回

  static size_t memory(
      MemoryUnit mu = MemoryUnit::KB_);  // 查询当前程序的内存使用量
};
}  // namespace profiler
}  // namespace toystl

#endif  // TOySTL_PROFILER_PROFILER_H_
