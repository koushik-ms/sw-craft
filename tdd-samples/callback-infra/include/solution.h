#include <chrono>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <thread>

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

class Worker {
public:
  virtual bool operator==(Worker const &other) const { return true; }
  virtual void schedule(Duration period, CallbackFunction callback) {
    throw std::runtime_error("Not Implemented!");
  }
  virtual void cancel() { throw std::runtime_error("Not Implemented!"); }
  virtual ~Worker(){};
};

class CallbackInfrastructureImpl : public CallbackInfrastructure {
public:
  CallbackInfrastructureImpl() = default;
  using FactoryMethodType = std::function<Worker *()>;
  CallbackInfrastructureImpl(FactoryMethodType factory) : factory_{factory} {}
  IdType registerCallback(Duration duration,
                          CallbackFunction callback) override {
    worker_ = factory_();
    return 0;
  };

private:
  Worker *worker_;
  FactoryMethodType factory_;
};

class Infomap {
public:
  int add(Worker const &w) { throw std::runtime_error("Not Implemented"); }
  Worker remove(int id) { throw std::runtime_error("Not Implemented"); };

private:
  Worker w_;
};

class StdThreadWorker {
public:
  void schedule(Duration period, CallbackFunction callback) {
    callback_thread = new std::thread([period, callback]() {
      std::this_thread::sleep_for(period);
      callback();
    });
  }
  void cancel() {
    //
  }

private:
  std::thread *callback_thread; // TODO: replace with smart ptr
};

template <typename T = StdThreadWorker> class WorkerImpl : public Worker {
public:
  WorkerImpl(T &&impl = T()) : impl_(std::forward<T>(impl)) {}
  void schedule(Duration period, CallbackFunction callback) override {
    impl_.schedule(period, callback);
  };
  void cancel() override { impl_.cancel(); };

private:
  T impl_;
};
