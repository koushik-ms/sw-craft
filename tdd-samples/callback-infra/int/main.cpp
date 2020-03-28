#include <chrono>
#include <iostream>
#include <thread>

#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// ON/OFF test suites
bool TEST_WORKER = true;
bool TEST_CALLBACKINFRA = false;
bool TEST_INTEGRATION = false;

TEST_SUITE_BEGIN("CallbackInfra integration scenario" *
                 doctest::skip(!TEST_INTEGRATION));

TEST_CASE("Building an instance of CallbackInfra") {
  SUBCASE("Able to create an instance with default tick") {}
  SUBCASE("Cannot build instance without specifying tick") {}
}

TEST_CASE("What (de)Register is made-up of") {
  using namespace std::chrono_literals;
  // this test works out the internals of the register method
  auto period = 500ms;
  auto callback = []() {};
  Worker original;

  original.schedule(period, callback);
  original.cancel();
  // Above leads to a unit-test for CallbackInfrastructure
  //     Make mock worker
  //     Create a CallbackInfrastructure-like object `sut` with mock worker as
  //     dependency Call register on sut, expect call to worker with right
  //     param values. Call de-register on sut, expect call to cancel.
}
TEST_CASE("How to keep track of registrations") {
  using namespace std::chrono_literals;
  auto period = 500ms;
  auto callback = []() {};
  Infomap m;
  Worker original;

  original.schedule(period, callback);
  auto id = m.add(original);
  CHECK(id != 0);

  Worker retrieved = m.remove(id);
  CHECK(retrieved == original);
}
TEST_SUITE_END(); // "CallbackInfra integration scenario"

TEST_SUITE_BEGIN("CallbackInfrastructureImpl Unit tests" *
                 doctest::skip(!TEST_CALLBACKINFRA));
TEST_CASE("Can call register") {
  using namespace std::chrono_literals;
  CallbackInfrastructure *cIn = new CallbackInfrastructureImpl();
  cIn->registerCallback(100ms, []() {});
  delete cIn;
}
TEST_CASE("CallbackInfra Should use worker factory to process registrations") {
  using namespace std::chrono_literals;
  CallbackInfrastructure *cIn =
      new CallbackInfrastructureImpl([]() { return Worker(); });
  cIn->registerCallback(100ms, []() {});
  delete cIn;
}
TEST_SUITE_END(); // CallbackInfrastructureImpl Unit tests

TEST_SUITE_BEGIN("Worker Unit tests" * doctest::skip(!TEST_WORKER));

struct MockCallbackProvider {
  void doSomething() {
    std::cout << "Callback didSomething!" << std::endl;
    didSomething = true;
    callCount++;
  }
  bool didSomething{false};
  unsigned callCount{0};
};

class WorkerShould {

protected:
  WorkerShould() {
    using namespace std::chrono_literals;
    w = new WorkerImpl<>();
    REQUIRE(w != nullptr);
    arbitrary_period = 100ms;
    arbitrary_wait_time = arbitrary_period + 100ms;
  }
  virtual ~WorkerShould() { delete w; }
  Worker *w;
  Duration arbitrary_period;
  Duration arbitrary_wait_time;
};

TEST_CASE_FIXTURE(WorkerShould, "schedule a callback with local lambda") {
  // TODO: Need a timeout on this test-case not to exceed 2.5 periods
  unsigned called{0};
  auto fn = [&called]() {
    std::cout << "Callback!" << std::endl;
    ++called;
  };
  w->schedule(arbitrary_period, fn);
  std::this_thread::sleep_for(arbitrary_wait_time);
  CHECK(called > 0);
}

TEST_CASE_FIXTURE(WorkerShould,
                  "schedule a callback with free function/ class member func") {
  MockCallbackProvider mock;
  auto memberFunc = std::bind(&MockCallbackProvider::doSomething, &mock);
  CHECK(!mock.didSomething);
  CHECK(mock.callCount == 0);
  w->schedule(arbitrary_period, memberFunc);
  std::this_thread::sleep_for(arbitrary_wait_time);
  CHECK(mock.callCount > 0);
  CHECK(mock.didSomething);
}

TEST_CASE_FIXTURE(WorkerShould, "cancel the currently schedule callback") {
  w->cancel();
}
TEST_CASE_FIXTURE(WorkerShould, "invoke Callback repeatedly until cancelled") {
  unsigned called{0};
  unsigned multiple{3};
  auto fn = [&called]() {
    std::cout << "Callback!" << std::endl;
    ++called;
  };
  w->schedule(arbitrary_period, fn);
  std::this_thread::sleep_for(multiple * arbitrary_wait_time);
  CHECK(called > 0);
  // CHECK(called >= multiple); // TODO: Fix this
}
TEST_CASE_FIXTURE(WorkerShould,
                  "return from Schedule() immediately (within 1ms)") {}

TEST_SUITE_END(); // "Worker Unit Tests"
