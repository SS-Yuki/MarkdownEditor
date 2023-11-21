#include "md_text.h"

#include <climits>
#include <iostream>
#include <sstream>

#include "md_elem.h"
#include "utils.h"

MdText::MdText(const std::string& elem_str) {
  auto pos = elem_str.find_first_of(' ', 0);
  int len = elem_str.length();
  pos = (pos >= len) ? len : pos;
  mdes_.prefix = elem_str.substr(0, pos);
  mdes_.content = elem_str.substr(pos, len - pos);
  mdes_.content.erase(0, mdes_.content.find_first_not_of(' '));
  mdes_.str = elem_str;
  level_ = INT_MAX;
}

MdText::~MdText() = default;

auto MdText::content() const -> std::string { return mdes_.content; }

auto MdText::HasChild() -> bool { return false; }

auto MdText::Add(std::shared_ptr<MdElem> mdep) -> void {}

auto MdText::Display(int depth, int flag) -> void {
  PrintTreeElem(mdes_.content, depth, flag);
}

auto MdText::Clear() -> void {}