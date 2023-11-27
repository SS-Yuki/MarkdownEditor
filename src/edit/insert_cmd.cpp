#include "insert_cmd.h"

#include <string>
#include <vector>

#include "md_file.h"
#include "utils.h"

InsertCmd::InsertCmd(const std::shared_ptr<MdFile>& mdf,
                     const std::string& command_str,
                     const std::vector<std::string>& parms) {
  mdf_ = mdf;
  command_str_ = command_str;
  parms_ = parms;
  if (IsNumber(parms[1])) {
    index_ = std::stoi(parms[1]);
    int pos = command_str.find(parms[2]);
    elem_str_ = command_str.substr(pos, command_str.size() - pos);
  } else {
    index_ = INT_MAX;
    int pos = command_str.find(parms[1]);
    elem_str_ = command_str.substr(pos, command_str.size() - pos);
  }
}
InsertCmd::~InsertCmd() = default;
auto InsertCmd::Exec() -> bool {
  return mdf_->InsertInList(index_, elem_str_, true, &unsaved_);
}
auto InsertCmd::Undo() -> void {
  mdf_->DeleteInList(index_);
  mdf_->set_unsaved(unsaved_);
}
auto InsertCmd::Redo() -> void { mdf_->InsertInList(index_, elem_str_, true); }
