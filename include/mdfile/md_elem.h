#pragma once

#include <list>
#include <memory>
#include <string>
#include <utility>

struct MdElemString {
  std::string content;
  std::string prefix;
  std::string str;
};

class MdElem {
 public:
  explicit MdElem() = default;
  virtual ~MdElem() = default;
  virtual auto HasChild() -> bool = 0;
  virtual auto Add(std::shared_ptr<MdElem> mdep) -> void = 0;
  virtual auto Clear() -> void = 0;
  // 树状展示
  virtual auto Display(int depth, int flag) -> void = 0;
  // 显示元素字符串
  [[nodiscard]] auto ShowElem() const -> MdElemString { return mdes_; };
  [[nodiscard]] auto Taller(const std::shared_ptr<MdElem>& mde) const -> bool {
    return level_ < mde->level();
  }
  [[nodiscard]] auto level() const -> int { return level_; }

 protected:
  MdElemString mdes_;
  int level_;
};