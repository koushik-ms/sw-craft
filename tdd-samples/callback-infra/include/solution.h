#include <chrono>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <thread>

#ifndef _CALLBACK_INFRA_SOLUTION_

#define _CALLBACK_INFRA_SOLUTION_

using Instant = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::system_clock::duration;
using CallbackFunction = std::function<void(void)>;

class CallbackInfrastructure {
public:
  using IdType = std::size_t;
  virtual IdType registerCallback(Duration duration,
                                  CallbackFunction callback) {
    throw std::runtime_error("Not Implemented!");
  }
  void deregisterCallback(int id){};
  virtual ~CallbackInfrastructure(){}; // this means we need move & copy ctors
                                       // too!
};

auto Now() { return std::chrono::system_clock::now(); }

class CallbackInfrastructureBuilder {
public:
  CallbackInfrastructureBuilder &anInstance() { return *this; }
  CallbackInfrastructureBuilder &WithDefaultTick() { return *this; }
  CallbackInfrastructure Build() { return CallbackInfrastructure{}; }
};

#endif
