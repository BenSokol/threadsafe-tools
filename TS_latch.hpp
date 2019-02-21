/**
* @Filename: TS_latch.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 19th, 2019 [12:28pm]
* @Modified: February 21st, 2019 [12:50am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_LATCH_HPP
#define TS_LATCH_HPP

#include <condition_variable>
#include <shared_mutex>

#if !defined(__cpp_lib_shared_mutex) && !defined(_LIBCPP_AVAILABILITY_SHARED_MUTEX)
#define shared_mutex shared_timed_mutex
#warning std::shared_mutex is not defined. Using std::shared_timed_mutex instead.
#endif

namespace TS {
  class Latch {
  public:
    explicit Latch(size_t aCount);
    ~Latch();

    void count_down_and_wait();
    void count_down(size_t n = 1);
    size_t get_count() const noexcept;
    void wait();

  private:
    std::shared_mutex mMtx;
    std::condition_variable_any mCond;
    size_t mThreshold;
    size_t mCount;
  };
}  // namespace TS

#if !defined(__cpp_lib_shared_mutex) && !defined(_LIBCPP_AVAILABILITY_SHARED_MUTEX)
#undef shared_mutex
#endif

#endif
