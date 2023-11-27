#pragma once

#include <memory>

#include "md_elem.h"
class MdTitle : public MdElem {
 public:
  MdTitle();
  ~MdTitle() override;
  explicit MdTitle(const std::string &elem_str);
  auto HasChild() -> bool override;
  auto Add(std::shared_ptr<MdElem> mdep) -> void override;
  auto Display(int depth, int flag) -> void override;
  auto Clear() -> void override;
  [[nodiscard]] auto content() const -> std::string;

 private:
  std::list<std::shared_ptr<MdElem>> children_list_;
};