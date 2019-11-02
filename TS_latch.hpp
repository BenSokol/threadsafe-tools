/**
* @Filename: TS_latch.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 19th, 2019 [12:28pm]
* @Modified: November 1st, 2019 [6:26pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_LATCH_HPP
#define TS_LATCH_HPP

#include <condition_variable>


#if __has_include(<shared_mutex>)
  #include <shared_mutex>
  #ifndef TS_LATCH_mutex_type
    #if defined(_LIBCPP_AVAILABILITY_SHARED_MUTEX)
      #define TS_LATCH_mutex_type std::shared_mutex
    #else
      #define TS_LATCH_mutex_type std::shared_timed_mutex
    #endif
  #endif
#else
  #error Requires #include <shared_mutex> for std::shared_mutex or std::shared_timed_mutex
#endif

namespace TS {
  class Latch {
  public:
    explicit Latch(size_t aCount);
    ~Latch();
    void clear_and_reset(size_t newThreshold);
    void reset(size_t newThreshold);
    void count_down_and_wait(size_t n = 1);
    void count_down(size_t n = 1);
    size_t get_count() const noexcept;
    void wait();

  private:
    TS_LATCH_mutex_type mMtx;
    std::condition_variable_any mCond;
    size_t mThreshold;
    size_t mCount;
  };
}  // namespace TS

#endif
