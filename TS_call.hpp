/**
* @Filename: TS_call.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [2:36pm]
* @Modified: February 21st, 2019 [12:40am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_CALL_HPP
#define TS_CALL_HPP

#include <mutex>

namespace TS {
  template <typename mtx_type, typename return_type, typename... Args>
  return_type call(mtx_type &mtx, return_type (*func)(Args...), Args... args) {
    std::lock_guard<std::mutex> lg(mtx);
    return func(args...);
  }

  template <typename mtx_type, typename... Args>
  void call(mtx_type &mtx, void (*func)(Args...), Args... args) {
    std::lock_guard<std::mutex> lg(mtx);
    func(args...);
  }
}  // namespace TS

#endif
