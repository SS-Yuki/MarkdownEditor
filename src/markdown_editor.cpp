#include "markdown_editor.h"

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "appendhead_cmd.h"
#include "appendtail_cmd.h"
#include "common.h"
#include "delete_cmd.h"
#include "edit_cmd.h"
#include "history_observer.h"
#include "insert_cmd.h"
#include "md_file.h"
#include "state_subject.h"
#include "stats_observer.h"
#include "utils.h"

MarkdownEditor::MarkdownEditor()
    : cur_file_no_(-1), last_command_(CommandType::kInvalid) {
  state_subject_ = std::make_shared<StateSubject>(mdfiles_);
  std::string history_path = std::string(LOG_PATH) + std::string(HISTORY_LOG);
  std::string stats_path = std::string(LOG_PATH) + std::string(STATS_LOG);
  state_subject_->Add(std::make_shared<HistoryObserver>(history_path));
  state_subject_->Add(std::make_shared<StatsObserver>(stats_path));
}

MarkdownEditor::~MarkdownEditor() = default;

auto MarkdownEditor::Launch() -> void {
  std::string command;
  std::vector<std::string> parms;
  while (std::getline(std::cin, command)) {
    parms = ParseInput(command);
    state_subject_->Change(command);
    if (!kCommandMap.contains(parms[0])) {
      last_command_ = CommandType::kInvalid;
      PrintErr("Invalid input!");
      continue;
    }
    const CommandType type = kCommandMap.at(parms[0]);
    switch (type) {
      case kLoad: {
        if (parms.size() < 2) {
          PrintErr("Usage: load 文件名");
          continue;
        }
        LoadFile(parms[1]);
        break;
      }
      case kSave: {
        SaveFile();
        break;
      }
      case kWs: {
        WorkspacesShow();
        break;
      }
      case kSwitch: {
        if (parms.size() < 2) {
          PrintErr("Usage: switch 文件序号");
          continue;
        }
        Switch(GetIntFromParms(parms, 1));
        break;
      }
      case kClose: {
        if (parms.size() < 2) {
          PrintErr("Usage: close 文件序号");
          continue;
        }
        CloseFile(GetIntFromParms(parms, 1));
        break;
      }

      case kInsert: {
        if (parms.size() < 2) {
          PrintErr("Usage: insert [n] 标题/文本");
        }
        Insert(command, parms);
        break;
      }
      case kAppendHead: {
        if (parms.size() < 2) {
          PrintErr("Usage: append-head 标题/文本");
        }
        AppendHead(command, parms);
        break;
      }
      case kAppendTail: {
        if (parms.size() < 2) {
          PrintErr("Usage: append-tail 标题/文本");
        }
        AppendTail(command, parms);
        break;
      }
      case kDelete: {
        if (parms.size() < 2) {
          PrintErr("Usage: delete 标题/文本 || delete 行号");
        }
        Delete(command, parms);
        break;
      }
      case kUndo: {
        Undo();
        break;
      }
      case kRedo: {
        Redo();
        break;
      }

      case kList: {
        List();
        break;
      }
      case kListTree: {
        ListTree();
        break;
      }
      case kDirTree: {
        std::string arg;
        if (parms.size() > 1) {
          arg = parms[1];
        }
        DirTree(arg);
        break;
      }

      case kHistory: {
        ShowHistory(GetIntFromParms(parms, 1));
        break;
      }
      case kStats: {
        std::string arg;
        if (parms.size() > 1) {
          arg = parms[1];
        }
        ShowStats(arg);
        break;
      }
      case kExit: {
        exit(0);
        break;
      }

      default: {
        break;
      }
    }
    last_command_ = type;
  }
}

auto MarkdownEditor::LoadFile(const std::string& url) -> void {
  std::fstream fs(url, std::ios::in);
  bool unsaved = false;
  if (!fs) {
    fs.open(url, std::ios::out);
    unsaved = true;
    if (!fs) {
      PrintErr("load path error");
      return;
    }
    fs.close();
  }

  std::filesystem::path load_path(url);
  for (auto&& mdf : mdfiles_) {
    if (std::filesystem::equivalent(std::filesystem::path(mdf->url()),
                                    load_path)) {
      PrintErr("file is opened");
      return;
    }
  }

  auto mdfp = std::make_shared<MdFile>(url, unsaved);
  if (cur_file_no_ != -1) {
    mdfiles_[cur_file_no_]->set_cur(false);
  }
  mdfiles_.push_back(mdfp);
  cur_file_no_ = mdfiles_.size() - 1;
  cur_file_url_ = url;
}

auto MarkdownEditor::SaveFile() -> void { mdfiles_[cur_file_no_]->Save(); }

auto MarkdownEditor::WorkspacesShow() -> void {
  int cnt = 1;
  for (auto&& mdf : mdfiles_) {
    std::cout << cnt++ << " " << mdf->WorkSpace() << std::endl;
  }
}

auto MarkdownEditor::Switch(int n) -> void {
  n--;
  if (n < 0 || n >= mdfiles_.size()) {
    PrintErr("(switch) [n] is not exist");
    return;
  }
  if (cur_file_no_ == n) {
    return;
  }
  mdfiles_[cur_file_no_]->set_cur(false);

  mdfiles_[n]->set_cur(true);
  cur_file_no_ = n;
}

auto MarkdownEditor::CloseFile(int n) -> void {
  n--;
  if (n < 0 || n >= mdfiles_.size()) {
    PrintErr("(close) [n] is not exist");
    return;
  }
  if (mdfiles_[n]->unsaved()) {
    while (true) {
      std::cout << "save changed file before closing? (y/n)" << std::endl;
      std::string ans;
      std::getline(std::cin, ans);
      if (ans == "y") {
        mdfiles_[n]->Save();
        break;
      }
      if (ans == "n") {
        break;
      }
      PrintErr("Invalid input");
    }
  }

  if (cur_file_no_ > n) {
    // 0 1- 2 3<
    cur_file_no_--;
  } else if (cur_file_no_ == n) {
    if (cur_file_no_ > 0) {
      // 0 1 2 3<
      cur_file_no_--;
      mdfiles_[cur_file_no_]->set_cur(true);
    } else if (mdfiles_.size() == 1) {
      // 0<
      cur_file_no_--;
    } else {
      // 0< 1 2 3
      mdfiles_[cur_file_no_ + 1]->set_cur(true);
    }
  }
  mdfiles_.erase(mdfiles_.begin() + n);
}

auto MarkdownEditor::Insert(const std::string& command_str,
                            const std::vector<std::string>& parms) -> void {
  undo_map_[cur_file_url_] =
      std::make_shared<InsertCmd>(mdfiles_[cur_file_no_], command_str, parms);
  if (!undo_map_[cur_file_url_]->Exec()) {
    undo_map_.erase(cur_file_url_);
  }
}

auto MarkdownEditor::AppendHead(const std::string& command_str,
                                const std::vector<std::string>& parms) -> void {
  undo_map_[cur_file_url_] = std::make_shared<AppendHeadCmd>(
      mdfiles_[cur_file_no_], command_str, parms);
  if (!undo_map_[cur_file_url_]->Exec()) {
    undo_map_.erase(cur_file_url_);
  }
}

auto MarkdownEditor::AppendTail(const std::string& command_str,
                                const std::vector<std::string>& parms) -> void {
  undo_map_[cur_file_url_] = std::make_shared<AppendTailCmd>(
      mdfiles_[cur_file_no_], command_str, parms);
  if (!undo_map_[cur_file_url_]->Exec()) {
    undo_map_.erase(cur_file_url_);
  }
}

auto MarkdownEditor::Delete(const std::string& command_str,
                            const std::vector<std::string>& parms) -> void {
  undo_map_[cur_file_url_] =
      std::make_shared<DeleteCmd>(mdfiles_[cur_file_no_], command_str, parms);
  if (!undo_map_[cur_file_url_]->Exec()) {
    undo_map_.erase(cur_file_url_);
  }
}

auto MarkdownEditor::Undo() -> void {
  if (undo_map_.contains(cur_file_url_)) {
    undo_map_[cur_file_url_]->Undo();
    redo_map_[cur_file_url_] = undo_map_[cur_file_url_];
    undo_map_.erase(cur_file_url_);
  }
}

auto MarkdownEditor::Redo() -> void {
  if (redo_map_.contains(cur_file_url_)) {
    if (last_command_ == CommandType::kUndo) {
      redo_map_[cur_file_url_]->Redo();
    }
    redo_map_.erase(cur_file_url_);
  }
}

auto MarkdownEditor::List() -> void { mdfiles_[cur_file_no_]->List(); }

auto MarkdownEditor::ListTree() -> void { mdfiles_[cur_file_no_]->ListTree(); }

auto MarkdownEditor::DirTree(const std::string& dir) -> void {
  mdfiles_[cur_file_no_]->ListDirTree(dir);
}

auto MarkdownEditor::ShowHistory(int num) -> void {
  if (num < 0) {
    num = INT_MAX;
  }
  std::string history_path = std::string(LOG_PATH) + std::string(HISTORY_LOG);
  std::fstream log_file(history_path, std::ios::in);
  std::vector<std::string> log_items;
  std::string log_item;
  while (std::getline(log_file, log_item)) {
    if (log_item.find(SESSION_FLAG) != 0) {
      log_items.push_back(log_item);
    }
  }
  log_items.pop_back();
  int start = log_items.size() - num;
  start = (start < 0) ? 0 : start;
  for (int i = start; i < log_items.size(); i++) {
    std::cout << log_items[i] << std::endl;
  }
  log_file.close();
}

auto MarkdownEditor::ShowStats(const std::string& arg) -> void {
  if (arg == "all") {
    for (const auto& file : mdfiles_) {
      std::string interval =
          ShowTimeInterval(file->start_tp(), std::chrono::system_clock::now());
      std::cout << file->url() << " " << interval << std::endl;
    }
  } else {
    if (mdfiles_.empty()) {
      return;
    }
    std::string interval = ShowTimeInterval(mdfiles_[cur_file_no_]->start_tp(),
                                            std::chrono::system_clock::now());
    std::cout << mdfiles_[cur_file_no_]->url() << " " << interval << std::endl;
  }
}
