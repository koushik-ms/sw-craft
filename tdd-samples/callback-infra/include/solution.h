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
  virtual void deregisterCallback(IdType id) {
    throw std::runtime_error("Not Implemented!");
  };
  virtual ~CallbackInfrastructure(){}; // TODO: this means we need move & copy
                                       // ctors too!
};

using CallbackInfrastructurePtr = std::shared_ptr<CallbackInfrastructure>;

inline auto Now() { return std::chrono::system_clock::now(); }

class CallbackInfrastructureBuilder {
public:
  virtual CallbackInfrastructureBuilder *anInstance() { return this; }
  virtual CallbackInfrastructureBuilder *WithDefaultTick() { return this; }
  virtual CallbackInfrastructureBuilder *WithTick(Duration const &tick) {
    return this;
  }
  virtual CallbackInfrastructure BuildPrototype() {
    return CallbackInfrastructure{};
  }
  virtual CallbackInfrastructurePtr Build() {
    throw std::runtime_error("Not yet Implemented");
  }
};

using CallbackInfrastructureBuilderPtr =
    std::shared_ptr<CallbackInfrastructureBuilder>;
CallbackInfrastructureBuilderPtr getCallbackInfrastructureBuilder();

#endif
