#include <chrono>
#include <iostream>
#include <thread>

#include "implementation.h"
#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// For mocking, this has to be after the doctest.h include
#include "doctest/trompeloeil.hpp"

constexpr bool TEST_CALLBACKINFRA{true};

TEST_SUITE_BEGIN("CallbackInfrastructureImpl Unit tests" *
                 doctest::skip(!TEST_CALLBACKINFRA));

struct MockWorker : public Worker {
  MAKE_MOCK2(schedule, void(Duration, CallbackFunction), override);
  MAKE_MOCK0(cancel, void(void), override);
};

TEST_CASE("CallbackInfra: register spec" *
          doctest::description(
              "Should use worker factory to create new worker per "
              "registration. Should call schedule on worker instance.")) {
  using namespace std::chrono_literals;
  using trompeloeil::_;
  unsigned worker_instance_count{0};
  MockWorker mock;
  Duration a_period{350ms};
  CallbackFunction a_cb = []() {};
  std::unique_ptr<CallbackInfrastructure> sut =
      std::make_unique<CallbackInfrastructureImpl>(
          [&worker_instance_count, &mock]() {
            ++worker_instance_count;
            return &mock;
          });
  {
    REQUIRE_CALL(mock, schedule(_, _)).TIMES(2);
    auto id1 = sut->registerCallback(a_period, a_cb);
    auto id2 = sut->registerCallback(a_period * 2, a_cb);
    REQUIRE(id1 != id2);
  }
  REQUIRE(worker_instance_count == 2);
}
TEST_CASE("register returns diff id for diff period same callback") {
  using trompeloeil::_;
  MockWorker mock;
  Duration a_period{500000000};
  CallbackFunction a_cb = []() {};
  std::unique_ptr<CallbackInfrastructure> sut =
      std::make_unique<CallbackInfrastructureImpl>([&mock]() { return &mock; });
  {
    ALLOW_CALL(mock, schedule(_, _));
    auto id1 = sut->registerCallback(a_period, a_cb);
    auto id2 = sut->registerCallback(a_period * 2, a_cb);
    REQUIRE(id1 != id2);
  }
}
TEST_CASE("register returns diff id for same period diff callback") {}
TEST_CASE("allow de-register of callback") {}
TEST_CASE("disallow de-register of invalid callback") {}
TEST_CASE(
    "silently fail when de-registering a callback that is already "
    "de-registered") {}
TEST_CASE("CallbackInfra shoud de-register all callbacks when destroyed") {}
TEST_CASE("register shall return within 1ms") {}
TEST_SUITE_END();  // CallbackInfrastructureImpl Unit tests
