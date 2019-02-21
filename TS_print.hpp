/**
* @Filename: TS_print.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [2:36pm]
* @Modified: February 21st, 2019 [2:17am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_PRINT_HPP
#define TS_PRINT_HPP

#include <iostream>
#include <mutex>

#include "private/TS_ostream.hpp"

namespace TS {

  template <typename mtx_type, typename T>
  void print(mtx_type& mtx, T t) {
    TS::PRIVATE::ostream(std::cout, mtx, t);
  }

  template <typename mtx_type, typename T, typename... Args>
  void print(mtx_type& mtx, T t, Args... args) {
    TS::PRIVATE::ostream(std::cout, mtx, t, args...);
  }

}  // namespace TS

#endif
