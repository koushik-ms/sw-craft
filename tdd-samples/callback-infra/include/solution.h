#include <chrono>
#include <functional>

using Instant = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::system_clock::duration;
using CallbackFunction = std::function<void(void)>;

class CallbackInfrastructure {
public:
  virtual int registerCallback(Duration duration, CallbackFunction callback) {
    return 0;
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

class Worker {
public:
  virtual bool operator==(Worker const &other) const { return true; }
  virtual void schedule(Duration a, CallbackFunction b) {}
  virtual void cancel() {}
  virtual ~Worker(){};
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

template <typename T> class WorkerImpl : public Worker {
  //
};

class StdThreadWorker {};
