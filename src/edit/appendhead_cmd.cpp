#include "appendhead_cmd.h"

#include <string>
#include <vector>

#include "md_file.h"

AppendHeadCmd::AppendHeadCmd(const std::shared_ptr<MdFile>& mdf,
                             const std::string& command_str,
                             const std::vector<std::string>& parms) {
  mdf_ = mdf;
  command_str_ = command_str;
  parms_ = parms;
  index_ = 1;
  int pos = command_str.find(parms[1]);
  elem_str_ = command_str.substr(pos, command_str.size() - pos);
}
AppendHeadCmd::~AppendHeadCmd() = default;
auto AppendHeadCmd::Exec() -> bool {
  return mdf_->InsertInList(index_, elem_str_, true, &unsaved_);
}
auto AppendHeadCmd::Undo() -> void {
  mdf_->DeleteInList(index_);
  mdf_->set_unsaved(unsaved_);
}
auto AppendHeadCmd::Redo() -> void {
  mdf_->InsertInList(index_, elem_str_, true);
}
