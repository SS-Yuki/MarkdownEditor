#pragma once

#include "md_elem.h"
class MdText : public MdElem {
 public:
  explicit MdText(const std::string &elem_str);
  ~MdText() override;
  auto HasChild() -> bool override;
  auto Add(std::shared_ptr<MdElem> mdep) -> void override;
  auto Clear() -> void override;
  auto Display(int depth, int flag) -> void override;
  [[nodiscard]] auto content() const -> std::string;
};
