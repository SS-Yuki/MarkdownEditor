#include "history_observer.h"

#include "common.h"
#include "utils.h"

HistoryObserver::HistoryObserver(const std::string& log_path) {
  log_stream_ = std::fstream(log_path, std::ios::app);
  if (!log_stream_.is_open()) {
    PrintErr("open logfile fail");
  } else {
    log_stream_ << "session start at " << GetCurTime() << std::endl;
  }
}
HistoryObserver::~HistoryObserver() {
  if (log_stream_.is_open()) {
    log_stream_.close();
  }
}
auto HistoryObserver::Respose(const State& state) -> void {
  log_stream_ << GetCurTime() << " " << state.command << std::endl;
}
