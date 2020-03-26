#include <chrono>
#include <functional>
#include <stdexcept>
#include <thread>

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
  virtual void schedule(Duration period, CallbackFunction callback) {
    throw std::runtime_error("Not Implemented!");
  }
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

class StdThreadWorker {
public:
  void schedule(Duration period, CallbackFunction callback) {
    std::thread callback_thread([period, callback]() {
      std::this_thread::sleep_for(period);
      callback();
    });
  }
};

template <typename T = StdThreadWorker> class WorkerImpl : public Worker {
public:
  WorkerImpl(T const &impl = T()) : impl_(std::move(impl)) {}
  virtual void schedule(Duration period, CallbackFunction callback) override {
    impl_.schedule(period, callback);
  };

private:
  T impl_;
};
