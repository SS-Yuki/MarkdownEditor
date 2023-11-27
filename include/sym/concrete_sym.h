#pragma once

#include <memory>
#include <string>
#include <utility>

#include "base_sym.h"

template <typename T>
class UnsavedSym : public BaseSym<T> {
 public:
  explicit UnsavedSym(std::unique_ptr<BaseSym<T>> base_sym)
      : base_sym_(std::move(base_sym)) {}
  auto Show(const T* obj) const -> std::string override {
    return base_sym_->Show(obj) + "*";
  }

 private:
  std::unique_ptr<BaseSym<T>> base_sym_;
};

template <typename T>
class CurSym : public BaseSym<T> {
 public:
  explicit CurSym(std::unique_ptr<BaseSym<T>> base_sym)
      : base_sym_(std::move(base_sym)) {}
  auto Show(const T* obj) const -> std::string override {
    return base_sym_->Show(obj) + "<";
  }

 private:
  std::unique_ptr<BaseSym<T>> base_sym_;
};
