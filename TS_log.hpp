/**
* @Filename: TS_log.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 21st, 2019 [2:16am]
* @Modified: February 21st, 2019 [2:19am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_LOG_HPP
#define TS_LOG_HPP

#include <fstream>
#include <mutex>

#include "private/TS_ostream.hpp"

namespace TS {

  template <typename mtx_type, typename T>
  void log(std::ofstream &ofs, mtx_type &mtx, T t) {
    TS::PRIVATE::ostream(ofs, mtx, t);
  }

  template <typename mtx_type, typename T, typename... Args>
  void log(std::ofstream &ofs, mtx_type &mtx, T t, Args... args) {
    TS::PRIVATE::ostream(ofs, mtx, t, args...);
  }

}  // namespace TS

#endif
