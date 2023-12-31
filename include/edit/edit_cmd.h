#pragma once

#include <string>
#include <vector>

class MdFile;
class EditCmd {
 public:
  virtual auto Exec() -> bool = 0;
  virtual auto Undo() -> void = 0;
  virtual auto Redo() -> void = 0;
  virtual ~EditCmd() = default;

 protected:
  std::shared_ptr<MdFile> mdf_;
  std::string command_str_;
  std::vector<std::string> parms_;
  std::string elem_str_;
  int index_;
  bool unsaved_;
};