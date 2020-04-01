#include "solution.h"

#include "implementation.h"

class CallbackInfrastructureBuilderImpl : public CallbackInfrastructureBuilder {
  CallbackInfrastructurePtr Build() override {
    worker_factory_ = []() { return std::make_shared<WorkerImpl<>>(); };
    return std::make_shared<CallbackInfrastructureImpl>(
        []() { return std::make_shared<WorkerImpl<>>(); });
  }

 private:
  CallbackInfrastructureImpl::FactoryMethodType worker_factory_;
};

CallbackInfrastructureBuilderPtr getCallbackInfrastructureBuilder() {
  return std::make_shared<CallbackInfrastructureBuilderImpl>();
}
