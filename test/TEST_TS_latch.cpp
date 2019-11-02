/**
* @Filename: TEST_TS_latch.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  November 1st, 2019 [5:20pm]
* @Modified: November 1st, 2019 [7:24pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <iostream>
#include <thread>
#include <vector>

#include <unistd.h>

#include "TS_latch.hpp"
#include "catch.hpp"


TEST_CASE("TS::Latch non-threaded test cases", "[TS::Latch]") {
  TS::Latch latch(3);

  REQUIRE(latch.get_count() == 3);

  SECTION("count_down works correctly") {
    for (int8_t i = 2; i >= 0; --i) {
      latch.count_down();
      REQUIRE(latch.get_count() == i);
    }
  }
  SECTION("count_down works correctly decrementing > 1") {
    latch.count_down(2);
    REQUIRE(latch.get_count() == 1);
  }
  SECTION("count_down_and_wait does not hang when count is 0") {
    latch.count_down(2);
    REQUIRE(latch.get_count() == 1);
    latch.count_down_and_wait();
    REQUIRE(latch.get_count() == 0);
  }
  SECTION("wait does not hang when count is 0") {
    latch.count_down(3);
    REQUIRE(latch.get_count() == 0);
    latch.wait();
  }
  SECTION("reset works correctly") {
    latch.count_down();
    REQUIRE(latch.get_count() == 2);
    latch.reset(5);
    REQUIRE(latch.get_count() == 5);
    latch.count_down();
    REQUIRE(latch.get_count() == 4);
  }
  SECTION("reset works correctly") {
    latch.count_down();
    REQUIRE(latch.get_count() == 2);
    latch.reset(5);
    REQUIRE(latch.get_count() == 5);
    latch.count_down();
    REQUIRE(latch.get_count() == 4);
  }
}


void TEST_latch_helper(TS::Latch *latch);
void TEST_latch_helper(TS::Latch *latch) {
  sleep(1);
  latch->count_down();
}

TEST_CASE("TS::Latch threaded test cases", "[TS::Latch]") {
  TS::Latch *latch = new TS::Latch(3);

  SECTION("wait works correctly") {
    std::thread t1(TEST_latch_helper, latch);
    std::thread t2(TEST_latch_helper, latch);
    std::thread t3(TEST_latch_helper, latch);

    latch->wait();
    REQUIRE(latch->get_count() == 0);

    t1.join();
    t2.join();
    t3.join();

    delete latch;
  }
  SECTION("count_down_and_wait works correctly") {
    std::thread t1(TEST_latch_helper, latch);
    std::thread t2(TEST_latch_helper, latch);

    latch->count_down_and_wait();
    REQUIRE(latch->get_count() == 0);

    t1.join();
    t2.join();

    delete latch;
  }
}
