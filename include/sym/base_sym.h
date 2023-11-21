#pragma once

#include <string>

template <typename T>
class BaseSym {
 public:
  virtual auto Show(const T* obj) const -> std::string = 0;
  virtual ~BaseSym() = default;
};