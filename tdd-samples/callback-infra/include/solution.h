#include <chrono>
#include <functional>

using Instant = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::system_clock::duration;

class CallbackInfrastructure {
public:
  template <typename B> int registerCallback(Duration duration, B callback) {
    return 0;
  }
  void deregisterCallback(int id){};
};

auto Now() { return std::chrono::system_clock::now(); }

class CallbackInfrastructureBuilder {
public:
  CallbackInfrastructureBuilder &anInstance() { return *this; }
  CallbackInfrastructureBuilder &WithDefaultTick() { return *this; }
  CallbackInfrastructure Build() { return CallbackInfrastructure{}; }
};

class Worker {
public:
  bool operator==(Worker const &other) const { return true; }
  template <typename B> void schedule(Duration a, B b) {}
  void cancel() {}
};

class Infomap {
public:
  int add(Worker const &w) {
    w_ = w;
    return 0;
  }
  Worker remove(int id) { return w_; };

private:
  Worker w_;
};

class CallbackInfrastructureImpl : public CallbackInfrastructure {
public:
  CallbackInfrastructureImpl() = default;
  using FactoryMethodType = std::function<Worker(void)>;
  CallbackInfrastructureImpl(FactoryMethodType factory) : factory_{factory} {}

private:
  FactoryMethodType factory_;
};
