#pragma once
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common.h"
class Observer;
class Mdfile;
class StateSubject {
 public:
  explicit StateSubject(std::vector<std::shared_ptr<MdFile>>& mdfiles);
  auto Add(const std::shared_ptr<Observer>& observer) -> void;
  auto Remove(const std::shared_ptr<Observer>& observer) -> void;
  auto NotifyObservers(const State& state) -> void;
  auto Change(const std::string& command) -> void;

 private:
  State state_;
  std::list<std::shared_ptr<Observer>> observers_;
};