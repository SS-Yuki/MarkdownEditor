#include "stats_observer.h"

#include "common.h"
#include "md_file.h"
#include "utils.h"

StatsObserver::StatsObserver(const std::string& log_path) {
  {
    log_stream_ = std::fstream(log_path, std::ios::app);
    if (!log_stream_.is_open()) {
      PrintErr("open logfile fail");
    } else {
      log_stream_ << "session start at " << GetCurTime() << std::endl;
    }
  }
}

StatsObserver::~StatsObserver() {
  if (log_stream_.is_open()) {
    log_stream_.close();
  }
}
auto StatsObserver::Respose(const State& state) -> void {
  std::vector<std::string> parms = ParseInput(state.command);

  if (!kCommandMap.contains(parms[0])) {
    return;
  }
  if (kCommandMap.at(parms[0]) == CommandType::kClose) {
    if (parms.size() < 2) {
      return;
    }
    int index = atoi(parms[1].c_str()) - 1;
    if (index < 0 || index >= state.mdfiles.size()) {
      return;
    }
    std::string interval = ShowTimeInterval(state.mdfiles[index]->start_tp(),
                                            std::chrono::system_clock::now());
    log_stream_ << state.mdfiles[index]->url() << " " << interval << std::endl;
  }
  if (kCommandMap.at(parms[0]) == CommandType::kExit) {
    for (const auto& mdfile : state.mdfiles) {
      std::string interval = ShowTimeInterval(mdfile->start_tp(),
                                              std::chrono::system_clock::now());
      log_stream_ << mdfile->url() << " " << interval << std::endl;
    }
  }
}