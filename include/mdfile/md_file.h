#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "base_sym.h"
#include "elem_list.h"
#include "md_elem.h"

class MdFile {
 public:
  explicit MdFile(const std::string& url, bool unsaved);
  ~MdFile();
  auto Save() -> void;
  auto List() -> void;
  auto ListTree() -> void;
  auto ListDirTree(const std::string& dir) -> void;
  auto UpdateTree() -> void;
  [[nodiscard]] auto WorkSpace() const -> std::string;
  [[nodiscard]] auto url() const -> std::string;
  [[nodiscard]] auto unsaved() const -> bool;
  auto set_unsaved(bool unsaved) -> void;
  auto set_cur(bool cur) -> void;
  /*
  InsertInList
  modify: insert是否造成了修改(初始化的插入不造成修改)
  return: 返回insert前的unsaved_
  */
  auto InsertInList(int index, const std::string& elem_str, bool modify = true,
                    bool* unsaved = nullptr) -> bool;
  /*
  DeleteInList
  elem_str: 存入被删掉行的elem_str
  return: 返回insert前的unsaved_
  */
  auto DeleteInList(int index, std::string* elem_str = nullptr,
                    bool* unsaved = nullptr) -> bool;
  /*
  DeleteInList
  index: 存入被删掉行的行号
  return: 返回insert前的unsaved_
  */
  auto DeleteInList(const std::string& content, int* index = nullptr,
                    std::string* elem_str = nullptr, bool* unsaved = nullptr)
      -> bool;

  //  private:
  bool unsaved_;
  bool cur_;
  std::string url_;
  std::unique_ptr<ElemList> list_;
  std::unique_ptr<MdElem> root_;
  class UrlShow : public BaseSym<MdFile> {
   public:
    [[nodiscard]] auto Show(const MdFile* mdf) const -> std::string override;
  };
  static auto GenerateMdElem(const std::string& elem_str)
      -> std::shared_ptr<MdElem>;
};