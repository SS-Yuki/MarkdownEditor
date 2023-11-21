#pragma once

#include <string>
auto PrintErr(const std::string& info) -> void;
auto PrintTreeElem(const std::string& elem_str, int depth, int flag) -> void;
auto ParseInput(const std::string& str) -> std::vector<std::string>;
auto IsNumber(const std::string& str) -> bool;