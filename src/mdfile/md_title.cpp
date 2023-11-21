#include "md_title.h"

#include <iostream>
#include <sstream>

#include "md_elem.h"
#include "utils.h"

MdTitle::MdTitle() { level_ = 0; }

MdTitle::MdTitle(const std::string& elem_str) {
  auto pos = elem_str.find_first_of(' ', 0);
  int len = elem_str.length();
  pos = (pos >= len) ? len : pos;
  mdes_.prefix = elem_str.substr(0, pos);
  mdes_.content = elem_str.substr(pos, len - pos);
  mdes_.content.erase(0, mdes_.content.find_first_not_of(' '));
  mdes_.str = elem_str;
  level_ = pos;
}

MdTitle::~MdTitle() = default;

auto MdTitle::content() const -> std::string { return mdes_.content; }

auto MdTitle::HasChild() -> bool { return true; }

auto MdTitle::Add(std::shared_ptr<MdElem> mdep) -> void {
  children_list_.push_back(mdep);
}

auto MdTitle::Display(int depth, int flag) -> void {
  if (level_ != 0) {
    PrintTreeElem(mdes_.content, depth, flag);
  }
  for (const auto& child : children_list_) {
    if (child == children_list_.back()) {
      child->Display(depth + 1, flag | (1 << depth));
      continue;
    }
    child->Display(depth + 1, flag);
  }
}

auto MdTitle::Clear() -> void { children_list_.clear(); }