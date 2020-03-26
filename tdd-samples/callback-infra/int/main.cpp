#include <chrono>
#include <iostream>
#include <thread>

#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_SUITE("Building an instance of CallbackInfra") {
  TEST_CASE("Able to create an instance with default tick") {}
  TEST_CASE("Cannot build instance without specifying tick") {}
}

TEST_SUITE("CallbackInfra registration scenario" * doctest::skip()) {
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
}

TEST_SUITE("CallbackInfrastructureImpl Unit tests" * doctest::skip()) {
  TEST_CASE("Can call register") {
    using namespace std::chrono_literals;
    CallbackInfrastructure *cIn = new CallbackInfrastructureImpl();
    cIn->registerCallback(100ms, []() {});
    delete cIn;
  }
  TEST_CASE(
      "CallbackInfra Should use worker factory to process registrations") {
    using namespace std::chrono_literals;
    CallbackInfrastructure *cIn =
        new CallbackInfrastructureImpl([]() { return Worker(); });
    cIn->registerCallback(100ms, []() {});
    delete cIn;
  }
}

TEST_SUITE("Worker Unit tests") {
  TEST_CASE("Can assert") { REQUIRE(true); }
  TEST_CASE("Can create a worker") {
    Worker *w = new WorkerImpl<StdThreadWorker>();
    delete w;
  }
  TEST_CASE("Can schedule a callback with local lambda") {
    // TODO: Need a timeout on this test-case not to exceed 2.5 periods
    using namespace std::chrono_literals;
    Worker *w = new WorkerImpl<StdThreadWorker>();
    auto arbitrary_period{500ms};
    volatile unsigned called{0};
    w->schedule(arbitrary_period, [&called]() {
      std::cout << "Callback!" << std::endl;
      ++called;
    });
    std::this_thread::sleep_for(arbitrary_period * 2);
    CHECK(called > 0);
    delete w;
  }
  TEST_CASE("Can schedule a callback with free function/ class member func") {}
  TEST_CASE("Can cancel the currently schedule callback") {}
  TEST_CASE("Schedule should return immediately (within 1ms)") {}
}
