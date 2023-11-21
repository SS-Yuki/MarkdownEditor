#pragma once

#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "edit_cmd.h"
#include "md_file.h"

enum CommandType {
  kLoad,
  kSave,
  kWs,
  kSwitch,
  kClose,
  kInsert,
  kAppendHead,
  kAppendTail,
  kDelete,
  kUndo,
  kRedo,
  kList,
  kListTree,
  kDirTree,
  kHistory,
  kStats
};

class MarkdownEditor {
 public:
  MarkdownEditor();
  ~MarkdownEditor();
  auto Launch() -> void;

  //  private:
  std::vector<std::shared_ptr<MdFile>> mdfiles_;
  std::unordered_map<std::string, std::shared_ptr<EditCmd>> undo_map_;
  std::unordered_map<std::string, std::shared_ptr<EditCmd>> redo_map_;
  int cur_file_no_;
  std::string cur_file_url_;
  const static std::unordered_map<std::string, CommandType> kCommandMap;

  auto LoadFile(const std::string& url) -> void;
  auto SaveFile() -> void;
  auto WorkspacesShow() -> void;
  auto Switch(int n) -> void;
  auto CloseFile(int n) -> void;

  auto Insert(const std::string& command_str,
              const std::vector<std::string>& parms) -> void;
  auto AppendHead(const std::string& command_str,
                  const std::vector<std::string>& parms) -> void;
  auto AppendTail(const std::string& command_str,
                  const std::vector<std::string>& parms) -> void;
  auto Delete(const std::string& command_str,
              const std::vector<std::string>& parms) -> void;
  auto Undo() -> void;
  auto Redo() -> void;

  auto List() -> void;
  auto ListTree() -> void;
  auto DirTree(const std::string& dir) -> void;

  auto ShowHistory() -> void;
  auto ShowStats() -> void;
};
