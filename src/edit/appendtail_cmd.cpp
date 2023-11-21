#include "appendtail_cmd.h"

#include <string>
#include <vector>

#include "utils.h"

AppendTailCmd::AppendTailCmd(const std::shared_ptr<MdFile>& mdf,
                             const std::string& command_str,
                             const std::vector<std::string>& parms) {
  mdf_ = mdf;
  command_str_ = command_str;
  parms_ = parms;
  index_ = INT_MAX;
  int pos = command_str.find(parms[1]);
  elem_str_ = command_str.substr(pos, command_str.size() - pos);
}
AppendTailCmd::~AppendTailCmd() = default;
auto AppendTailCmd::Exec() -> bool {
  return mdf_->InsertInList(index_, elem_str_, true, &unsaved_);
}
auto AppendTailCmd::Undo() -> void {
  mdf_->DeleteInList(index_);
  mdf_->set_unsaved(unsaved_);
}
auto AppendTailCmd::Redo() -> void {
  mdf_->InsertInList(index_, elem_str_, true);
}
