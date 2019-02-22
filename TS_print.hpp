/**
* @Filename: TS_print.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [2:36pm]
* @Modified: February 22nd, 2019 [1:23pm]
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

  template <typename mtx_type, typename... Args>
  void print(mtx_type& mtx, Args... args) {
    std::lock_guard<mtx_type> lck(mtx);
    TS::PRIVATE::ostream(std::cout, args...);
  }

}  // namespace TS

#endif
