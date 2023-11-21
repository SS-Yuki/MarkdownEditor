#pragma once

#include <memory>
#include <string>
#include <vector>

#include "edit_cmd.h"
#include "md_file.h"
class DeleteCmd : public EditCmd {
 public:
  DeleteCmd(const std::shared_ptr<MdFile>& mdf, const std::string& command_str,
            const std::vector<std::string>& parms);
  ~DeleteCmd() override;
  auto Exec() -> bool override;
  auto Undo() -> void override;
  auto Redo() -> void override;

  // private:
  std::string content_;
};