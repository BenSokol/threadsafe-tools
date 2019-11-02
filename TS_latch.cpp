/**
* @Filename: TS_latch.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 19th, 2019 [12:28pm]
* @Modified: November 1st, 2019 [7:13pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <condition_variable>
#include <shared_mutex>

#include "TS_latch.hpp"


namespace TS {

  Latch::Latch(size_t aCount) : mThreshold(aCount), mCount(aCount) {
    if (aCount == 0) {
      throw std::invalid_argument("aCount must be greater than zero.");
    }
  }

  Latch::~Latch() {
    mCond.notify_all();
  }

  void Latch::clear_and_reset(size_t newThreshold) {
    std::shared_lock<TS_LATCH_mutex_type> lck(mMtx);
    if (newThreshold == 0) {
      throw std::invalid_argument("newThreshold must be greater than zero.");
    }
    mCond.notify_all();
    mThreshold = newThreshold;
    mCount = newThreshold;
  }

  void Latch::reset(size_t newThreshold) {
    std::shared_lock<TS_LATCH_mutex_type> lck(mMtx);
    if (newThreshold == 0) {
      throw std::invalid_argument("newThreshold must be greater than zero.");
    }
    mThreshold = newThreshold;
    mCount = newThreshold;
  }

  void Latch::count_down_and_wait(size_t n) {
    std::shared_lock<TS_LATCH_mutex_type> lck(mMtx);
    if (n > mCount) {
      mCount = 0;
    }
    else {
      mCount = mCount - n;
    }

    if (mCount == 0) {
      mCond.notify_all();
    }
    else {
      mCond.wait(lck);
    }
  }

  void Latch::count_down(size_t n) {
    std::shared_lock<TS_LATCH_mutex_type> lck(mMtx);
    if (n > mCount) {
      mCount = 0;
    }
    else {
      mCount = mCount - n;
    }

    if (mCount == 0) {
      mCond.notify_all();
    }
  }

  size_t Latch::get_count() const noexcept {
    return mCount;
  }

  void Latch::wait() {
    std::shared_lock<TS_LATCH_mutex_type> lck(mMtx);
    if (mCount == 0) {
      return;
    }
    mCond.wait(lck);
  }

}  // namespace TS
