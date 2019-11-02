/**
* @Filename: TS_ostream.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 21st, 2019 [2:08am]
* @Modified: November 1st, 2019 [7:06pm]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_OSTREAM_HPP
#define TS_OSTREAM_HPP

#if !defined(TS_LOGANDPRINT_HPP) && !defined(TS_PRINT_HPP) && !defined(TS_LOG_HPP) && !defined(TEST_BUILD)
  #error Internal use only. TS_ostream.hpp is private. Do not include outside of threadsafe-tools
#endif

#include <ostream>

namespace TS {
  namespace PRIVATE {
    template <typename T>
    void ostream(std::ostream &os, T t) {
      os << t << std::flush;
    }

    template <typename T, typename... Args>
    void ostream(std::ostream &os, T t, Args... args) {
      os << t << std::flush;
      ostream(os, args...);
    }
  }  // namespace PRIVATE

}  // namespace TS

#endif
