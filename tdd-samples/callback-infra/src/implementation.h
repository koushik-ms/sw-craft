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
