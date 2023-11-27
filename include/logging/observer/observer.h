#pragma once

struct State;
class Observer {
 public:
  virtual ~Observer() = default;
  virtual auto Respose(const State& state) -> void = 0;
};