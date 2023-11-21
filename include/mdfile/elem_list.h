#pragma once

#include <list>
#include <memory>
#include <string>

#include "md_elem.h"
class ElemList {
 public:
  ElemList();
  auto list() -> std::list<std::shared_ptr<MdElem>>;
  /*
  InsertElem
  index: INT_MAX->尾插
  return: bool(index不正确)
  */
  auto InsertElem(int index, const std::shared_ptr<MdElem>& mdep) -> bool;
  /*
  DeleteElem
  return: bool(index不正确)
  */
  auto DeleteElem(int index) -> bool;
  /*
  DeleteElem
  content: 不包含(#|*|数字)
  return: bool(未搜索到content)
  */
  auto DeleteElem(const std::string& content) -> bool;
  /*
  GetElem
  elem_str: 存入目标行的elem_str
  return: bool(index不正确)
  */
  auto GetElem(int index, std::string* elem_str) -> bool;
  /*
  SearchContent
  index: 存入具有目标内容的index
  return: bool(未搜索到content)
  */
  auto SearchContent(const std::string& content, int* index) -> bool;

 private:
  std::list<std::shared_ptr<MdElem>> list_;
};