#include <chrono>
#include <iostream>
#include <thread>

#include "implementation.h"
#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// For mocking, this has to be after the doctest.h include
#include "doctest/trompeloeil.hpp"

constexpr bool TEST_WORKER{true};
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
    arbitrary_period = 10ms;
    arbitrary_wait_time = arbitrary_period + 1ms;
  }
  virtual ~WorkerShould() {}
  Duration arbitrary_period;
  Duration arbitrary_wait_time;
};

TEST_CASE_FIXTURE(WorkerShould, "schedule a callback with local lambda" *
                                    doctest::timeout(2.0)) {
  // TODO: Need a timeout on this test-case not to exceed 2.5 periods
  auto worker = WorkerImpl{};
  unsigned called{0};
  auto fn = [&called]() {
    std::cout << "Callback!" << std::endl;
    ++called;
  };
  worker.schedule(arbitrary_period, fn);
  std::this_thread::sleep_for(arbitrary_wait_time);
  CHECK(called > 0);
}

TEST_CASE_FIXTURE(WorkerShould,
                  "schedule a callback with free function/ class member func" *
                      doctest::timeout(2.0)) {
  MockCallbackProvider mock;
  auto worker = WorkerImpl{};
  auto memberFunc = std::bind(&MockCallbackProvider::doSomething, &mock);
  CHECK(!mock.didSomething);
  CHECK(mock.callCount == 0);
  worker.schedule(arbitrary_period, memberFunc);
  std::this_thread::sleep_for(arbitrary_wait_time);
  CHECK(mock.callCount > 0);
  CHECK(mock.didSomething);
}

TEST_CASE_FIXTURE(WorkerShould, "cancel the currently schedule callback") {
  auto worker = WorkerImpl{};
  unsigned called{0};
  auto fn = [&called]() {
    std::cout << "Callback!" << std::endl;
    ++called;
  };
  worker.schedule(arbitrary_period, fn);
  std::this_thread::sleep_for(arbitrary_wait_time);
  CHECK(called > 0);
  auto total_called = called;
  worker.cancel();
  std::this_thread::sleep_for(arbitrary_period);
  CHECK(called <= total_called + 1);
}

TEST_CASE_FIXTURE(WorkerShould, "invoke Callback repeatedly until cancelled" *
                                    doctest::timeout(3.0)) {
  auto worker = WorkerImpl{};
  unsigned called{0};
  unsigned multiple{3};
  auto fn = [&called]() {
    std::cout << "Multiple Callback!" << std::endl;
    ++called;
  };
  worker.schedule(arbitrary_period, fn);
  std::this_thread::sleep_for(multiple * arbitrary_wait_time);
  CHECK(called > 0);
  CHECK(called >= multiple);
}
TEST_CASE_FIXTURE(WorkerShould,
                  "return from Schedule() immediately (within 1ms)") {
  using namespace std::chrono_literals;
  auto worker = WorkerImpl{};
  auto before = Now();
  worker.schedule(arbitrary_period, []() {});
  auto after = Now();
  CHECK((after - before) <= 1ms);
}

TEST_SUITE_END();  // "Worker Unit Tests"
