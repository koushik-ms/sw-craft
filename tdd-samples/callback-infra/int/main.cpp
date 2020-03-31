#include <chrono>
#include <iostream>
#include <thread>

#include "implementation.h"
#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// For mocking, this has to be after the doctest.h include
#include <doctest/trompeloeil.hpp>

// ON/OFF test suites (TODO: Make these static)
constexpr bool TEST_WORKER{true};
constexpr bool TEST_CALLBACKINFRA{true};
constexpr bool TEST_INTEGRATION{false};

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

TEST_SUITE_BEGIN("CallbackInfrastructureImpl Unit tests" *
                 doctest::skip(!TEST_CALLBACKINFRA));

class CallbackInfraShould {
 public:
  struct MockWorkerImpl {
    MockWorkerImpl() { ++instanceCount; }
    void schedule(Duration d, CallbackFunction callback) {
      schedule_called_ = true;
      ++schedule_count_;
    };
    void cancel() {
      cancel_called_ = true;
      ++cancel_count_;
    };
    bool schedule_called_{false}, cancel_called_{false};
    unsigned schedule_count_{};
    unsigned cancel_count_{};
    static unsigned instanceCount;
  };
  CallbackInfraShould() {
    auto worker_factory_method = []() {
      return new WorkerImpl<MockWorkerImpl>();
    };
    sut_ = new CallbackInfrastructureImpl(worker_factory_method);
  };
  ~CallbackInfraShould() { delete sut_; }

 protected:
  CallbackInfrastructure* sut_;
};
unsigned CallbackInfraShould::MockWorkerImpl::instanceCount{0};

TEST_CASE_FIXTURE(CallbackInfraShould, "allow register callback") {
  using namespace std::chrono_literals;
  sut_->registerCallback(100ms, []() {});
}
TEST_CASE_FIXTURE(CallbackInfraShould,
                  "use worker factory to process registrations") {
  using namespace std::chrono_literals;
  sut_->registerCallback(100ms, []() {});
  CHECK(CallbackInfraShould::MockWorkerImpl::instanceCount > 0);
}
TEST_CASE("CallbackInfra: register delegates correctly to worker schedule") {}
TEST_CASE("register returns diff id for diff period same callback") {}
TEST_CASE("register returns diff id for same period diff callback") {}
TEST_CASE("allow de-register of callback") {}
TEST_CASE("disallow de-register of invalid callback") {}
TEST_CASE(
    "silently fail when de-registering a callback that is already "
    "de-registered") {}
TEST_CASE("CallbackInfra shoud de-register all callbacks when destroyed") {}
TEST_CASE("register shall return within 1ms") {}
TEST_SUITE_END();  // CallbackInfrastructureImpl Unit tests

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
    arbitrary_period = 500ms;
    arbitrary_wait_time = arbitrary_period + 100ms;
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

// These are learning tests. By definition, this is bottom-up exploration of the
// libraries, language features that are used to engineer the solutions used to
// make the solution work.

#include <chrono>
#include <functional>
#include <string>
#include <type_traits>
#include <typeinfo>

TEST_SUITE_BEGIN("Learning tests");
TEST_CASE("Making an idtype for identifying registrations") {
  SUBCASE("Can make hash of a string") {
    std::string arbitrary_string{"Some text"};
    auto hasher = std::hash<std::string>{};
    auto the_hash = hasher(arbitrary_string);
    std::cout << "The has of " << arbitrary_string << " is: " << the_hash
              << std::endl;
  }
  SUBCASE("two strings have different hashes") {
    std::string first{"Some text1"};
    std::string second{"Some text2"};
    REQUIRE(first != second);

    auto hasher = std::hash<std::string>{};
    auto first_hash = hasher(first);
    auto second_hash = hasher(second);

    CHECK(first_hash != second_hash);
  }
  SUBCASE("hash of string is size_t") {
    auto hasher = std::hash<std::string>{};
    auto the_hash = hasher("Some string");
    std::cout << "The hash of \"Some string\" is :" << the_hash << std::endl;
    static_assert(std::is_same_v<std::size_t, decltype(the_hash)>,
                  "hasher output is not size_t");
  }
  using namespace std::chrono_literals;
  SUBCASE("Can convert a period and lambda into a string") {
    Duration d{500ms};
    auto f1 = []() {};
    auto rep = std::to_string(d.count()) + typeid(f1).name();
    static_assert(std::is_same_v<std::string, decltype(rep)>);
    CHECK(!rep.empty());
  }
  SUBCASE("Can convert unique period and lambda into unique hash") {
    Duration d1{500ms};
    Duration d2{200ms};
    Duration d3{500us};
    auto f1 = []() {};
    auto f2 = []() {};
    auto f3 = []() { std::cout << "a very important action\n"; };
    auto hasher = std::hash<std::string>{};
    auto make_hash = [&hasher](auto a, auto b) -> std::size_t {
      return hasher(std::to_string(a.count()) + typeid(b).name());
    };

    auto d1f1 = make_hash(d1, f1);
    auto d1f2 = make_hash(d1, f2);
    auto d1f3 = make_hash(d1, f3);

    auto d2f1 = make_hash(d2, f1);
    auto d2f2 = make_hash(d2, f2);
    auto d2f3 = make_hash(d2, f3);

    auto d3f1 = make_hash(d3, f1);
    auto d3f2 = make_hash(d3, f2);
    auto d3f3 = make_hash(d3, f3);

    auto all_hashes = {d1f1, d1f2, d1f3, d2f1, d2f2, d2f3, d3f1, d3f2, d3f3};
    std::cout << "Size of all_hashes: " << all_hashes.size() << "\n";
    for (auto const& name : all_hashes) {
      std::cout << name << std::endl;
    }
    std::set<decltype(d1f1)> duplicate_less(all_hashes.begin(),
                                            all_hashes.end());
    CHECK(all_hashes.size() == duplicate_less.size());
  }
}
TEST_SUITE_END();  // Learning tests
