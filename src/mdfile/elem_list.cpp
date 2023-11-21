#include "elem_list.h"

#include <climits>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <ostream>
#include <string>

#include "md_elem.h"

ElemList::ElemList() = default;
auto ElemList::InsertElem(int index, const std::shared_ptr<MdElem>& mdep)
    -> bool {
  if (index == INT_MAX) {
    index = list_.size() + 1;
  }
  if (index < 1 || index > list_.size() + 1) {
    return false;
  }
  auto pos = list_.begin();
  std::advance(pos, (index - 1));
  list_.insert(pos, mdep);
  return true;
}
auto ElemList::DeleteElem(int index) -> bool {
  if (index == INT_MAX) {
    index = list_.size();
  }
  if (index < 1 || index > list_.size() + 1) {
    return false;
  }
  auto pos = list_.begin();
  std::advance(pos, (index - 1));
  list_.erase(pos);
  return true;
}
auto ElemList::DeleteElem(const std::string& content) -> bool {
  for (auto it = list_.begin(); it != list_.end();) {
    std::string str = it->get()->ShowElem().content;
    if (str == content) {
      it = list_.erase(it);
      return true;
    }
    it++;
  }
  return false;
}
auto ElemList::GetElem(int index, std::string* elem_str) -> bool {
  if (index == INT_MAX) {
    index = list_.size();
  }
  if (index < 1 || index > list_.size() + 1) {
    return false;
  }
  auto pos = list_.begin();
  std::advance(pos, (index - 1));
  if (elem_str != nullptr) {
    *elem_str = pos->get()->ShowElem().str;
  }
  return true;
}
auto ElemList::SearchContent(const std::string& content, int* index) -> bool {
  int cnt = 1;
  for (auto& it : list_) {
    std::string str = it.get()->ShowElem().content;
    if (str == content) {
      if (index != nullptr) {
        *index = cnt;
      }
      return true;
    }
    cnt++;
  }
  return false;
}

auto ElemList::list() -> std::list<std::shared_ptr<MdElem>> { return list_; }
