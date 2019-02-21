/**
* @Filename: TS_ostream.hpp
* @Author:   Ben Sokol <Ben>
* @Email:    ben@bensokol.com
* @Created:  February 21st, 2019 [2:08am]
* @Modified: February 21st, 2019 [2:17am]
* @Version:  1.0.0
*
* Copyright (C) 2019 by Ben Sokol. All Rights Reserved.
*/

#ifndef TS_OSTREAM_HPP
#define TS_OSTREAM_HPP

#include <mutex>
#include <ostream>

namespace TS {
  namespace PRIVATE {
    template <typename T>
    void _ostream(std::ostream &os, T t) {
      os << t << std::flush;
    }

    template <typename T, typename... Args>
    void _ostream(std::ostream &os, T t, Args... args) {
      os << t << std::flush;
      _ostream(os, args...);
    }

    template <typename mtx_type, typename T>
    void ostream(std::ostream &os, mtx_type &mtx, T t) {
      mtx.lock();
      os << t << std::flush;
      mtx.unlock();
    }

    template <typename mtx_type, typename T, typename... Args>
    void ostream(std::ostream &os, mtx_type &mtx, T t, Args... args) {
      mtx.lock();
      os << t << std::flush;
      _ostream(os, args...);
      mtx.unlock();
    }
  }  // namespace PRIVATE

}  // namespace TS

#endif
