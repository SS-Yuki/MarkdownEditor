#pragma once
#include <fstream>
#include <string>

#include "observer.h"

class HistoryObserver : public Observer {
 public:
  explicit HistoryObserver(const std::string& log_path);
  ~HistoryObserver() override;
  auto Respose(const State& state) -> void override;

 private:
  std::fstream log_stream_;
};