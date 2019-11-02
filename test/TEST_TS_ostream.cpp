/**
* @Filename: TEST_TS_ostream.cpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  November 1st, 2019 [5:18pm]
* @Modified: November 1st, 2019 [7:23pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#include <cstdint>

#include <sstream>
#include <string>
#include <thread>

#include "TS_ostream.hpp"
#include "catch.hpp"

TEST_CASE("TS::PRIVATE::ostream", "[TS::PRIVATE::ostream]") {
  std::stringstream ss;
  REQUIRE(ss.str().empty() == true);

  SECTION("ostream works for single int") {
    TS::PRIVATE::ostream(ss, 10);
    REQUIRE(ss.str() == "10");
  }
  SECTION("ostream works for single string") {
    TS::PRIVATE::ostream(ss, "string");
    REQUIRE(ss.str() == "string");
  }
  SECTION("ostream works for single bool") {
    TS::PRIVATE::ostream(ss, false);
    REQUIRE(ss.str() == "0");
  }
  SECTION("ostream works for single double") {
    TS::PRIVATE::ostream(ss, 1.5);
    REQUIRE(ss.str() == "1.5");
  }
  SECTION("ostream works for single char") {
    TS::PRIVATE::ostream(ss, 'c');
    REQUIRE(ss.str() == "c");
  }
  SECTION("ostream works with many arguments") {
    TS::PRIVATE::ostream(
      ss, "This", ' ', "is", " a number: ", 10, ", and 10 == 10 is: ", (10 == 10 ? "true" : "false"), "\n");
    REQUIRE(ss.str() == "This is a number: 10, and 10 == 10 is: true\n");
  }
}
