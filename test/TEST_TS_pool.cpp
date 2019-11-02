/**
* @Filename: TEST_TS_pool.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  November 1st, 2019 [5:18pm]
* @Modified: November 1st, 2019 [7:23pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>

#include <thread>

#include "TS_pool.hpp"
#include "catch.hpp"

static const size_t TEST_COUNT = 10000;
static const std::vector<size_t> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
static const size_t result = 55;

volatile std::atomic<size_t> total = 0;
volatile size_t sum[TEST_COUNT];

std::mutex mtx;

void TEST_pool_helper(size_t thread);
void TEST_pool_helper(size_t thread) {
  sum[thread] = 0;
  for (size_t i = 0; i < vec.size(); ++i) {
    sum[thread] += vec[i];
  }
  mtx.lock();
  total += sum[thread];
  mtx.unlock();
}

TEST_CASE("TS::pool", "[TS::pool]") {
  TS::pool<uint8_t> pool(4);
  SECTION("Thread pool initializes and reports empty") {
    REQUIRE(pool.remainingTasks() == 0);
  }
  SECTION("wait does not hang when pool is empty") {
    pool.wait();
    REQUIRE(pool.remainingTasks() == 0);
  }
  SECTION("enqueue works correctly for 1 function") {
    total = 0;
    pool.enqueue(0, &TEST_pool_helper, 0);
    pool.wait();
    REQUIRE(pool.remainingTasks() == 0);
    REQUIRE(total == sum[0]);
    REQUIRE(total == result);
  }
  SECTION("enqueue works correctly for 10000 function calls") {
    size_t answer = 0;
    total = 0;
    for (size_t i = 0; i < TEST_COUNT; ++i) {
      pool.enqueue(i % 10, &TEST_pool_helper, i);
      answer += result;
    }
    pool.wait();
    REQUIRE(pool.remainingTasks() == 0);
    REQUIRE(total == answer);
    for (size_t i = 0; i < TEST_COUNT; ++i) {
      REQUIRE(sum[i] == result);
    }
  }
}
