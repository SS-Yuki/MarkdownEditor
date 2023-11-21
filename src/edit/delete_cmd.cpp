#include "delete_cmd.h"

#include <climits>
#include <string>
#include <vector>

#include "utils.h"

DeleteCmd::DeleteCmd(const std::shared_ptr<MdFile>& mdf,
                     const std::string& command_str,
                     const std::vector<std::string>& parms) {
  mdf_ = mdf;
  command_str_ = command_str;
  parms_ = parms;
  if (IsNumber(parms[1])) {
    index_ = std::stoi(parms[1]);
  } else {
    index_ = INT_MIN;
    int pos = command_str.find(parms[1]);
    content_ = command_str.substr(pos, command_str.size() - pos);
  }
}
DeleteCmd::~DeleteCmd() = default;
auto DeleteCmd::Exec() -> bool {
  bool succ;
  if (index_ != INT_MIN) {
    succ = mdf_->DeleteInList(index_, &elem_str_, &unsaved_);
  } else {
    succ = mdf_->DeleteInList(content_, &index_, &elem_str_, &unsaved_);
  }
  return succ;
}
auto DeleteCmd::Undo() -> void {
  mdf_->InsertInList(index_, elem_str_);
  mdf_->set_unsaved(unsaved_);
}
auto DeleteCmd::Redo() -> void { unsaved_ = mdf_->DeleteInList(index_); }
