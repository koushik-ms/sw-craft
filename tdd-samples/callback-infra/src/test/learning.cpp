// These are learning tests. By definition, this is bottom-up exploration of the
// libraries, language features that are used to engineer the solutions used to
// make the solution work.

#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <type_traits>
#include <typeinfo>

#include "implementation.h"
#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// For mocking, this has to be after the doctest.h include
#include "doctest/trompeloeil.hpp"

constexpr bool TEST_LEARNING{true};

TEST_SUITE_BEGIN("Learning tests" * doctest::skip(!TEST_LEARNING));
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
  SUBCASE("hash of string is of type size_t") {
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
    auto make_hash = [&hasher](auto const& a, auto const& b) -> std::size_t {
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
  SUBCASE("Can make a random ID") {
    // Standard mersenne_twister_engine seeded with std::random_device
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<CallbackInfrastructure::IdType> dis;
    CallbackInfrastructure::IdType x = dis(generator);
    CallbackInfrastructure::IdType y = dis(generator);
    std::cout << "First: " << x << " Second: " << y << std::endl;
    CHECK(x != y);
  }
}
TEST_SUITE_END();  // Learning tests
