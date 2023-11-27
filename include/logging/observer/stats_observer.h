#pragma once
#include <fstream>

#include "observer.h"

class StatsObserver : public Observer {
 public:
  explicit StatsObserver(const std::string& log_path);
  ~StatsObserver() override;
  auto Respose(const State& state) -> void override;

 private:
  std::fstream log_stream_;
};