#include "md_file.h"

#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>

#include "concrete_sym.h"
#include "elem_list.h"
#include "md_text.h"
#include "md_title.h"

MdFile::MdFile(const std::string& url, bool unsaved)
    : url_(url),
      unsaved_(unsaved),
      cur_(true),
      start_tp_(std::chrono::system_clock::now()) {
  root_ = std::make_unique<MdTitle>();
  list_ = std::make_unique<ElemList>();
  std::ifstream ifs(url, std::ios::in);
  std::string elem_str;
  while (!ifs.eof()) {
    std::getline(ifs, elem_str);
    if (elem_str.empty()) {
      continue;
    }
    InsertInList(INT_MAX, elem_str, false);
  }
  ifs.close();
}

MdFile::~MdFile() = default;
auto MdFile::Save() -> void {
  unsaved_ = false;
  std::fstream fs(url_, std::ios::out);
  for (const auto& elem : list_->list()) {
    fs << elem->ShowElem().str << std::endl;
  }
  fs.close();
}
auto MdFile::List() -> void {
  for (const auto& mdep : list_->list()) {
    std::cout << mdep->ShowElem().str << std::endl;
  }
}
auto MdFile::ListTree() -> void {
  UpdateTree();
  root_->Display(0, 0);
}
auto MdFile::ListDirTree(const std::string& dir) -> void {
  UpdateTree();
  auto list = list_->list();
  for (const auto& mdep : list) {
    std::string str = mdep.get()->ShowElem().content;
    if (str == dir) {
      mdep->Display(0, 0);
      return;
    }
  }
}
auto MdFile::UpdateTree() -> void {
  root_->Clear();
  std::stack<std::shared_ptr<MdElem>> st;
  for (const auto& mdep : list_->list()) {
    while (!st.empty() && !st.top()->Taller(mdep)) {
      st.pop();
    }
    if (st.empty()) {
      root_->Add(mdep);
    } else {
      st.top()->Add(mdep);
    }
    st.push(mdep);
    mdep->Clear();
  }
}
auto MdFile::url() const -> std::string { return url_; }
auto MdFile::unsaved() const -> bool { return unsaved_; }
auto MdFile::set_unsaved(bool unsaved) -> void { unsaved_ = unsaved; }
auto MdFile::set_cur(bool cur) -> void { cur_ = cur; }
auto MdFile::start_tp() -> std::chrono::system_clock::time_point {
  return start_tp_;
}
auto MdFile::UrlShow::Show(const MdFile* mdf) const -> std::string {
  return mdf->url();
}
auto MdFile::WorkSpace() const -> std::string {
  std::unique_ptr<BaseSym<MdFile>> bsp = std::make_unique<UrlShow>();
  if (unsaved_) {
    bsp = std::make_unique<UnsavedSym<MdFile>>(std::move(bsp));
  }
  if (cur_) {
    bsp = std::make_unique<CurSym<MdFile>>(std::move(bsp));
  }
  return bsp->Show(this);
}

auto MdFile::InsertInList(int index, const std::string& elem_str, bool modify,
                          bool* unsaved) -> bool {
  if (unsaved != nullptr) {
    *unsaved = unsaved_;
  }
  std::shared_ptr<MdElem> mde = GenerateMdElem(elem_str);
  bool succ = list_->InsertElem(index, mde);
  unsaved_ = unsaved_ || (modify && succ);
  return succ;
}

auto MdFile::DeleteInList(int index, std::string* elem_str, bool* unsaved)
    -> bool {
  if (unsaved != nullptr) {
    *unsaved = unsaved_;
  }
  bool exist = list_->GetElem(index, elem_str);
  if (!exist) {
    return false;
  }
  bool succ = list_->DeleteElem(index);
  unsaved_ = unsaved_ || succ;
  return true;
}
auto MdFile::DeleteInList(const std::string& content, int* index,
                          std::string* elem_str, bool* unsaved) -> bool {
  if (unsaved != nullptr) {
    *unsaved = unsaved_;
  }
  bool exist = list_->SearchContent(content, index);
  if (!exist) {
    return false;
  }
  list_->GetElem(*index, elem_str);
  bool succ = list_->DeleteElem(content);
  unsaved_ = unsaved_ || succ;
  return true;
}

auto MdFile::GenerateMdElem(const std::string& elem_str)
    -> std::shared_ptr<MdElem> {
  std::shared_ptr<MdElem> mdfp;
  if (elem_str[0] == '#') {
    mdfp = std::make_shared<MdTitle>(elem_str);
  } else {
    mdfp = std::make_shared<MdText>(elem_str);
  }
  return mdfp;
}