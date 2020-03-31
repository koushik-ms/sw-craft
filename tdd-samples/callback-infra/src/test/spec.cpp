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

auto return_new_worker = []() { return new Worker(); };

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
TEST_CASE("register returns diff id for same period diff callback") {
  using trompeloeil::_;
  MockWorker mock;
  Duration a_period{500000000};
  CallbackFunction a_cb = []() {};
  CallbackFunction another_cb = []() {
    std::cout << MakeId(Duration{200}, []() {}) << std::endl;
  };
  std::unique_ptr<CallbackInfrastructure> sut =
      std::make_unique<CallbackInfrastructureImpl>([&mock]() { return &mock; });
  {
    ALLOW_CALL(mock, schedule(_, _));
    auto id1 = sut->registerCallback(a_period, a_cb);
    auto id2 = sut->registerCallback(a_period, another_cb);
    REQUIRE(id1 != id2);
  }
}
TEST_CASE("allow de-register of callback. silently fail if invalid") {
  std::unique_ptr<CallbackInfrastructure> sut =
      std::make_unique<CallbackInfrastructureImpl>(return_new_worker);
  sut->deregisterCallback(0);
}
TEST_CASE("delegate de-register call to worker") {
  using trompeloeil::_;
  MockWorker mock;
  Duration a_period{500000000};
  CallbackFunction a_cb = []() {};

  std::unique_ptr<CallbackInfrastructure> sut =
      std::make_unique<CallbackInfrastructureImpl>([&mock]() { return &mock; });
  {
    ALLOW_CALL(mock, schedule(_, _));
    REQUIRE_CALL(mock, cancel());
    auto id = sut->registerCallback(a_period, a_cb);
    std::this_thread::sleep_for(a_period);
    sut->deregisterCallback(id);
  }
}
TEST_CASE("invoke cancel on the worker corresponding to callback") {
  using trompeloeil::_;
  MockWorker mock1, mock2;
  Duration a_period{500000000};
  CallbackFunction a_cb = []() {};
  bool mock_with_mock1 = true;
  std::unique_ptr<CallbackInfrastructure> sut =
      std::make_unique<CallbackInfrastructureImpl>(
          [&mock1, &mock2, &mock_with_mock1]() {
            std::cout << "Factory: " << std::boolalpha << mock_with_mock1
                      << ", " << &mock1 << ", " << &mock2 << std::endl;
            Worker *product;
            if (mock_with_mock1) {
              product = &mock1;
            } else {
              product = &mock2;
            }
            mock_with_mock1 = !mock_with_mock1;
            return product;
          });
  {
    ALLOW_CALL(mock1, schedule(_, _));
    ALLOW_CALL(mock2, schedule(_, _));
    REQUIRE_CALL(mock1, cancel());
    FORBID_CALL(mock2, cancel());
    std::cout << "Sut ready\nRegister first callback\n";
    auto id1 = sut->registerCallback(a_period, a_cb);
    std::cout << "Register second callback\n";
    auto id2 = sut->registerCallback(a_period * 2, a_cb);
    CHECK(id1 != id2);
    std::cout << "De-Register first callback\n";
    sut->deregisterCallback(id1);
  }
}
TEST_CASE("CallbackInfra shoud de-register all callbacks when destroyed") {}

TEST_CASE("register shall return within 1ms") {}
TEST_SUITE_END();  // CallbackInfrastructureImpl Unit tests
