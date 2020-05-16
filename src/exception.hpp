#ifndef PALETTE_CONVERTER_EXCEPTION_HPP
#define PALETTE_CONVERTER_EXCEPTION_HPP

#include <sstream>
#include <stdexcept>

#define EXCEPTION(msg) { \
  std::stringstream ss; \
  ss << msg; \
  throw std::runtime_error(ss.str()); \
}

#endif
