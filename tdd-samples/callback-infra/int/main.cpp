#include <chrono>
#include <memory>
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
  SUBCASE("Create a CallbackInfrastructure") {
    auto worker_factory = []() { return std::make_shared<WorkerImpl<>>(); };
    std::shared_ptr<CallbackInfrastructure> cIn =
        std::make_shared<CallbackInfrastructureImpl>(worker_factory);
    REQUIRE(cIn != nullptr);
    using namespace std::chrono_literals;
    auto period = 500ms;
    auto tick = 50ms;
    Instant registeredAt, calledAt;
    cIn->registerCallback(period, [&calledAt]() { calledAt = Now(); });
    registeredAt = Now();
    std::this_thread::sleep_for(period + tick + tick);
    auto latency = calledAt - registeredAt;
    CHECK(latency <= (period + tick));
    CHECK(latency >= (period - tick));
  }
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
  auto MakeId = [](Duration period,
                   CallbackFunction cb) -> CallbackInfrastructure::IdType {
    return std::hash<std::string>{}(std::to_string(period.count()) +
                                    typeid(cb).name());
  };

  WorkerPtr original_half{new WorkerImpl{}};
  CallbackInfrastructure::IdType id = MakeId(half_sec, callback_fn);
  data_store.insert({id, original_half});
  REQUIRE(data_store.find(id) != data_store.end());
  CHECK(data_store.find(id)->second == original_half);

  WorkerPtr original_one{new WorkerImpl{}};
  id = MakeId(one_sec, callback_fn);
  data_store.insert({id, original_one});

  CHECK(original_one != original_half);
  REQUIRE(data_store.find(id) != data_store.end());
  CHECK(data_store.find(id)->second == original_one);
}

TEST_SUITE_END(); // "CallbackInfra integration scenario"
