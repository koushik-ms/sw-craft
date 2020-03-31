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
  std::shared_ptr<Worker> original{new WorkerImpl{}};

  original->schedule(period, callback);
  original->cancel();
}

TEST_CASE("How to keep track of registrations") {
  using namespace std::chrono_literals;
  using WorkerPtr = std::shared_ptr<Worker>;
  auto one_sec = Duration{1s};
  auto half_sec = Duration{500ms};
  auto callback_fn = []() { std::cout << "Wonder!" << std::endl; };
  std::map<CallbackInfrastructure::IdType, WorkerPtr> data_store;

  WorkerPtr original_half{new WorkerImpl{}};
  original_half->schedule(half_sec, callback_fn);
  CallbackInfrastructure::IdType id = MakeId(half_sec, callback_fn);
  data_store.insert({id, original_half});
  REQUIRE(data_store.find(id) != data_store.end());
  CHECK(data_store.find(id)->second == original_half);

  WorkerPtr original_one{new WorkerImpl{}};
  original_one->schedule(one_sec, callback_fn);
  id = MakeId(one_sec, callback_fn);
  data_store.insert({id, original_one});

  CHECK(original_one != original_half);
  REQUIRE(data_store.find(id) != data_store.end());
  CHECK(data_store.find(id)->second == original_one);
}

TEST_SUITE_END();  // "CallbackInfra integration scenario"
