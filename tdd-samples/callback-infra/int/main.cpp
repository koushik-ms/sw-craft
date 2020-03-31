#include <chrono>
#include <thread>

#include "implementation.h"
#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// For mocking, this has to be after the doctest.h include
#include "doctest/trompeloeil.hpp"

constexpr bool TEST_INTEGRATION{true};

TEST_SUITE_BEGIN("CallbackInfra integration scenario" *
                 doctest::skip(!TEST_INTEGRATION) * doctest::may_fail());

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
  auto one_sec = Duration{1s};
  auto half_sec = Duration{500ms};
  auto callback_fn = []() { std::cout << "Wonder!" << std::endl; };
  std::map<CallbackInfrastructure::IdType, Worker> data_store;

  Worker original_half;
  original_half.schedule(half_sec, callback_fn);
  CallbackInfrastructure::IdType id = MakeId(half_sec, callback_fn);
  data_store.insert({id, original_half});
  REQUIRE(data_store.find(id) != data_store.end());
  CHECK(data_store.find(id)->second == original_half);

  Worker original_one;
  original_one.schedule(one_sec, callback_fn);
  id = MakeId(one_sec, callback_fn);
  data_store.insert({id, original_one});

  CHECK(original_one != original_half);
  REQUIRE(data_store.find(id) != data_store.end());
  CHECK(data_store.find(id)->second == original_one);
}

TEST_SUITE_END();  // "CallbackInfra integration scenario"
