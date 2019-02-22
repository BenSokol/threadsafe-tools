/**
* @Filename: TS_logAndPrint.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 22nd, 2019 [11:33am]
* @Modified: February 22nd, 2019 [1:21pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_LOGANDPRINT_HPP
#define TS_LOGANDPRINT_HPP

#include <fstream>
#include <iostream>
#include <mutex>

#include "private/TS_ostream.hpp"

#ifdef NDEBUG
#define TS_COUT_MUTEX_ARG_NAME
#else
#define TS_COUT_MUTEX_ARG_NAME printMtx
#endif

namespace TS {

  template <typename mtx_type, typename... Args>
  void logAndPrint(std::ofstream& ofs, mtx_type& TS_COUT_MUTEX_ARG_NAME, mtx_type& logMtx, Args... args) {
#ifdef NDEBUG
    std::lock_guard<mtx_type> lckLog(logMtx);
#else
    std::lock(TS_COUT_MUTEX_ARG_NAME, logMtx);
    std::lock_guard<mtx_type> lckPrint(printMtx, std::adopt_lock);
    std::lock_guard<mtx_type> lckLog(logMtx, std::adopt_lock);
    TS::PRIVATE::ostream(std::cout, args...);
#endif
    if (ofs.is_open()) {
      TS::PRIVATE::ostream(ofs, args...);
    }
  }

  template <typename mtx_type, typename... Args>
  void logAndPrintAlways(std::ofstream& ofs, mtx_type& printMtx, mtx_type& logMtx, Args... args) {
    std::lock(printMtx, logMtx);
    std::lock_guard<mtx_type> lckPrint(printMtx, std::adopt_lock);
    std::lock_guard<mtx_type> lckLog(logMtx, std::adopt_lock);
    TS::PRIVATE::ostream(std::cout, args...);
    if (ofs.is_open()) {
      TS::PRIVATE::ostream(ofs, args...);
    }
  }

}  // namespace TS

#undef TS_COUT_MUTEX_ARG_NAME

#endif
