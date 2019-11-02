/**
* @Filename: TS_call.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [2:36pm]
* @Modified: November 1st, 2019 [7:18pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_CALL_HPP
#define TS_CALL_HPP

#include <mutex>
#include <type_traits>

namespace TS {
  template <typename mtx_type, typename function_type, typename... Args>
  typename std::result_of<function_type(Args...)>::type call(mtx_type &mtx, function_type &&f, Args &&... args) {
    std::lock_guard<mtx_type> lg(mtx);
    return f(args...);
  }
}  // namespace TS

#endif
