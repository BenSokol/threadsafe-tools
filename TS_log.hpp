/**
* @Filename: TS_log.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 21st, 2019 [2:16am]
* @Modified: February 22nd, 2019 [1:23pm]
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

  template <typename mtx_type, typename... Args>
  void log(std::ofstream &ofs, mtx_type &mtx, Args... args) {
    std::lock_guard<mtx_type> lck(mtx);
    if (ofs.is_open()) {
      TS::PRIVATE::ostream(ofs, args...);
    }
  }

}  // namespace TS

#endif
