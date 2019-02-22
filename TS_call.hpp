/**
* @Filename: TS_call.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [2:36pm]
* @Modified: February 22nd, 2019 [2:21pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_CALL_HPP
#define TS_CALL_HPP

#error TS_call.hpp is not complete. DO NOT USE.

#include <functional>
#include <mutex>

namespace TS {
  // template <typename return_type, typename mtx_type>
  // return_type call(mtx_type &mtx, std::function<return_type()> func) {
  //   std::lock_guard<mtx_type> lg(mtx);
  //   return func();
  // }

  template <typename return_type, typename context_type, typename mtx_type, typename... Args>
  return_type call(mtx_type &mtx, context_type context, return_type (*func)(Args...), Args... args) {
    std::lock_guard<mtx_type> lg(mtx);
    return func(args...);
  }
}  // namespace TS

#endif
