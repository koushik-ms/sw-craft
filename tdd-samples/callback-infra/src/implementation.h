#include <future>
#include <iostream>
#include <map>
#include <random>
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
  using WorkerPtr = std::shared_ptr<Worker>;
  using FactoryMethodType = std::function<WorkerPtr()>;

  CallbackInfrastructureImpl() = default;
  CallbackInfrastructureImpl(FactoryMethodType factory) : factory_{factory} {}
  IdType registerCallback(Duration duration,
                          CallbackFunction callback) override {
    WorkerPtr worker{factory_()};
    IdType id = MakeRandomId();
    worker->schedule(duration, callback);
    registrations.insert({id, worker});
    return id;
  };

  void deregisterCallback(IdType id) override {
    if (registrations.find(id) != registrations.end()) {
      WorkerPtr worker = registrations.at(id);
      worker->cancel();
      registrations.erase(id);
    }
  }

  ~CallbackInfrastructureImpl() {
    std::cout << "At cleanup: " << registrations.size() << std::endl;
    for (auto &id_worker_pair : registrations) {
      WorkerPtr active = id_worker_pair.second;
      active->cancel();
    }
  }

private:
  IdType MakeRandomId() { return distribution(generator); }
  FactoryMethodType factory_{};
  std::mt19937 generator{std::random_device{}()};
  std::uniform_int_distribution<IdType> distribution;
  std::map<IdType, WorkerPtr> registrations;
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

template <typename T = AsyncWorker> class WorkerImpl : public Worker {
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

#endif
