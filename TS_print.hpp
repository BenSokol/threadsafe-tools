/**
* @Filename: TS_print.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [2:36pm]
* @Modified: February 21st, 2019 [12:40am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_PRINT_HPP
#define TS_PRINT_HPP

#include <iostream>
#include <mutex>

namespace TS {
  namespace PRIVATE {
    template <typename T>
    void print(T t) {
      std::cout << t << std::flush;
    }

    template <typename T, typename... Args>
    void print(T t, Args... args) {
      std::cout << t << std::flush;
      TS::PRIVATE::print(args...);
    }
  }  // namespace PRIVATE

  template <typename mtx_type, typename T>
  void print(mtx_type& mtx, T t) {
    mtx.lock();
    std::cout << t << std::flush;
    mtx.unlock();
  }

  template <typename mtx_type, typename T, typename... Args>
  void print(mtx_type& mtx, T t, Args... args) {
    mtx.lock();
    std::cout << t << std::flush;
    TS::PRIVATE::print(args...);
    mtx.unlock();
  }


}  // namespace TS

#endif
