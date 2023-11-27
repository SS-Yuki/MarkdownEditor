#include "state_subject.h"

#include "observer.h"

StateSubject::StateSubject(std::vector<std::shared_ptr<MdFile>>& mdfiles)
    : state_(State(mdfiles)) {}

auto StateSubject::Add(const std::shared_ptr<Observer>& observer) -> void {
  observers_.push_back(observer);
}
auto StateSubject::Remove(const std::shared_ptr<Observer>& observer) -> void {
  observers_.remove(observer);
}
auto StateSubject::NotifyObservers(const State& state) -> void {
  for (const auto& observer : observers_) {
    observer->Respose(state);
  }
}
auto StateSubject::Change(const std::string& command) -> void {
  state_.command = command;
  NotifyObservers(state_);
}
