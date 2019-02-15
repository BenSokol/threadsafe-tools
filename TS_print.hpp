/**
* @Filename: TS_print.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 15th, 2019 [2:36pm]
* @Modified: February 15th, 2019 [2:57pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_PRINT_HPP
#define TS_PRINT_HPP

#include <iostream>
#include <mutex>

namespace TS {

  // std::recursive_mutex
  template <typename T>
  void print(std::recursive_mutex &mtx, T t) {
    std::unique_lock<std::recursive_mutex> lck(mtx);
    std::cout << t;
    lck.unlock();
  }
  template <typename T, typename... Args>
  void print(std::recursive_mutex &mtx, T t, Args... args) {
    std::unique_lock<std::recursive_mutex> lck(mtx);
    std::cout << t;
    print(mtx, args...);
    lck.unlock();
  }


  // std::recursive_timed_mutex
  template <typename T>
  void print(std::recursive_timed_mutex &mtx, T t) {
    std::unique_lock<std::recursive_timed_mutex> lck(mtx);
    std::cout << t;
    lck.unlock();
  }
  template <typename T, typename... Args>
  void print(std::recursive_timed_mutex &mtx, T t, Args... args) {
    std::unique_lock<std::recursive_timed_mutex> lck(mtx);
    std::cout << t;
    print(mtx, args...);
    lck.unlock();
  }

  // std::unique_lock
  template <typename mtx_type, typename T>
  void print(std::unique_lock<mtx_type> &lck, T t) {
    lck.lock();
    std::cout << t;
    lck.unlock();
  }
  template <typename mtx_type, typename T, typename... Args>
  void print(std::unique_lock<mtx_type> &lck, T t, Args... args) {
    lck.lock();
    std::recursive_mutex rec_mtx;
    std::cout << t;
    print(rec_mtx, args...);
    lck.unlock();
  }


  // Any other type of mutex
  template <typename mtx_type, typename T>
  void print(mtx_type &mtx, T t) {
    std::unique_lock<mtx_type> lck(mtx);
    std::cout << t;
    lck.unlock();
  }
  template <typename mtx_type, typename T, typename... Args>
  void print(mtx_type &mtx, T t, Args... args) {
    std::unique_lock<mtx_type> lck(mtx);
    std::recursive_mutex rec_mtx;
    std::cout << t;
    print(rec_mtx, args...);
    lck.unlock();
  }

}  // namespace TS

#endif
