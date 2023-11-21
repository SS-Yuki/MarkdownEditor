#include "utils.h"

#include <iostream>
#include <sstream>

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