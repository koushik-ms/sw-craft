#include <future>
#include <iostream>
#include <typeinfo>

#include "solution.h"

#ifndef _CALLBACK_INFRA_IMPLEMENTATION_

#define _CALLBACK_INFRA_IMPLEMENTATION_
class Worker {
 public:
  virtual bool operator==(Worker const &other) const { return true; }
  virtual bool operator!=(Worker const &other) const { return false; }
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
    worker_->schedule(duration, callback);
    return 0;
  };

 private:
  Worker *worker_;
  FactoryMethodType factory_;
};

class AsyncWorker {
 public:
  void schedule(Duration period, CallbackFunction callback) {
    worker_fut = std::async([=]() {
      while (!stop) {
        std::this_thread::sleep_for(period);
        if (!stop) {
          callback();
        }
      }
    });
  }
  void cancel() {
    if (worker_fut.valid()) {
      std::cout << " cancel" << std::endl;
      stop = true;
      worker_fut.get();
    }
  }

  ~AsyncWorker() { cancel(); }

 private:
  std::future<void> worker_fut;
  bool stop{false};
};

template <typename T = AsyncWorker>
class WorkerImpl : public Worker {
 public:
  WorkerImpl() = default;
  WorkerImpl(T &&impl) : impl_(std::forward<T>(impl)) {}
  void schedule(Duration period, CallbackFunction callback) override {
    impl_.schedule(period, callback);
  };
  void cancel() override { impl_.cancel(); };

 private:
  T impl_;
};

CallbackInfrastructure::IdType MakeId(Duration period, CallbackFunction cb) {
  return std::hash<std::string>{}(std::to_string(period.count()) +
                                  typeid(cb).name());
}

// TODO: Put the Duration, Instant types in namespace and move the MakeId and
// Now() functions there.

#endif
