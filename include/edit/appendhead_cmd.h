#pragma once

#include <memory>
#include <string>
#include <vector>

#include "edit_cmd.h"
class MdFile;
class AppendHeadCmd : public EditCmd {
 public:
  AppendHeadCmd(const std::shared_ptr<MdFile>& mdf,
                const std::string& command_str,
                const std::vector<std::string>& parms);
  ~AppendHeadCmd() override;
  auto Exec() -> bool override;
  auto Undo() -> void override;
  auto Redo() -> void override;
};