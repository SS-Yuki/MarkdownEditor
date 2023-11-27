#include "utils.h"

#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

auto PrintErr(const std::string& info) -> void {
  std::cerr << "Error: " << info << std::endl;
}
auto PrintTreeElem(const std::string& elem_str, int depth, int flag) -> void {
  std::string str;
  for (int i = 0; i < depth - 1; i++) {
    str += (flag & (1 << i)) != 0 ? "    " : "|   ";
  }
  str += (flag & (1 << (depth - 1))) != 0 ? "└── " : "├── ";
  str += elem_str;
  std::cout << str << std::endl;
}
auto ParseInput(const std::string& str) -> std::vector<std::string> {
  std::istringstream iss(str);
  std::vector<std::string> parms;
  std::string parm;
  while (iss >> parm) {
    parms.push_back(parm);
  }
  std::string out;
  return parms;
}
auto IsNumber(const std::string& str) -> bool {
  for (char c : str) {
    if (std::isdigit(c) == 0) {
      return false;
    }
  }
  return true;
}

auto GetCurTime() -> std::string {
  auto tp = std::chrono::system_clock::now();
  time_t cur_time = std::chrono::system_clock::to_time_t(tp);
  char buf[100];
  std::strftime(buf, sizeof(buf), "%Y%m%d %H:%M:%S", std::localtime(&cur_time));
  return {buf};
}

auto ShowTimeInterval(std::chrono::system_clock::time_point start,
                      std::chrono::system_clock::time_point end)
    -> std::string {
  std::string ret;
  auto interval = std::chrono::duration_cast<std::chrono::seconds>(end - start);
  if (interval.count() >= 3600) {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(interval);
    interval -= hours;
    ret += std::to_string(hours.count()) + std::string(" 小时 ");
  }
  if (interval.count() >= 60) {
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(interval);
    interval -= mins;
    ret += std::to_string(mins.count()) + std::string(" 分钟 ");
  }
  ret += std::to_string(interval.count()) + std::string(" 秒 ");
  return ret;
}