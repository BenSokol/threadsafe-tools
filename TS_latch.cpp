/**
* @Filename: TS_latch.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 19th, 2019 [12:28pm]
* @Modified: February 21st, 2019 [1:41am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <condition_variable>
#include <shared_mutex>

#include "UTL_assert.h"

#include "TS_latch.hpp"

#if !defined(__cpp_lib_shared_mutex) && !defined(_LIBCPP_AVAILABILITY_SHARED_MUTEX)
#define shared_mutex shared_timed_mutex
#warning std::shared_mutex is not defined. Using std::shared_timed_mutex instead.
#endif

namespace TS {

  Latch::Latch(size_t aCount) : mThreshold(aCount), mCount(aCount) {
    if (aCount == 0) {
      throw std::invalid_argument("aCount must be greater than zero.");
    }
  }

  Latch::~Latch() {
    mCond.notify_all();
  }

  void Latch::count_down_and_wait() {
    std::shared_lock<std::shared_mutex> lck(mMtx);
    mCount = mCount - 1;
    if (mCount == 0) {
      mCount = mThreshold;
      mCond.notify_all();
      return;
    }

    mCond.wait(lck);
  }

  void Latch::count_down(size_t n) {
    std::shared_lock<std::shared_mutex> lck(mMtx);
    if (n < mCount) {
      throw std::invalid_argument("n must be less than count.");
    }
    if (mCount == 0) {
      mCount = mThreshold;
      mCond.notify_all();
    }
  }

  size_t Latch::get_count() const noexcept {
    return mCount;
  }

  void Latch::wait() {
    std::shared_lock<std::shared_mutex> lck(mMtx);
    mCond.wait(lck);
  }

}  // namespace TS