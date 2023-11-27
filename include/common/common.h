#pragma once
#include <string>
#include <vector>
#define LOG_PATH "/Users/yuki/Desktop/MarkdownEditor/logs/"
#define HISTORY_LOG "history.log"
#define STATS_LOG "stats.log"
class MdFile;
enum CommandType {
  kInvalid,
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
  kStats,
  kExit
};

const std::unordered_map<std::string, CommandType> kCommandMap = {
    {"load", CommandType::kLoad},
    {"save", CommandType::kSave},
    {"ws", CommandType::kWs},
    {"switch", CommandType::kSwitch},
    {"close", CommandType::kClose},

    {"insert", CommandType::kInsert},
    {"append-head", CommandType::kAppendHead},
    {"append-tail", CommandType::kAppendTail},
    {"delete", CommandType::kDelete},
    {"undo", CommandType::kUndo},
    {"redo", CommandType::kRedo},

    {"list", CommandType::kList},
    {"list-tree", CommandType::kListTree},
    {"dir-tree", CommandType::kDirTree},

    {"history", CommandType::kHistory},
    {"stats", CommandType::kStats},

    {"exit", CommandType::kExit}};

struct State {
  std::vector<std::shared_ptr<MdFile>>& mdfiles;
  std::string command;
  explicit State(std::vector<std::shared_ptr<MdFile>>& mdfiles)
      : mdfiles(mdfiles) {}
};