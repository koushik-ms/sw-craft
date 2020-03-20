#include <iostream>
#include <chrono>
#include <thread>

#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Can assert" * doctest::skip()) { CHECK(true); }

TEST_SUITE("CallbackInfra registration scenario" * doctest::skip())
{
    TEST_CASE("What Register is made-up of")
    {
        using namespace std::chrono_literals;
        // this test works out the internals of the register method
        auto period = 500ms;
        auto callback = []() {};
        Infomap m;
        Worker w;

        [[maybe_unused]] auto id = m.add(period, callback);
        w.schedule(period, callback);
        // CHECK(id == 0);
        // Make mock worker
        // Make mock infomap.
        // Create a CallbackInfrastructure-like object `sut` with mock worker as dependency
        // expect sut to call infomap with regn info
        // Expect worker to call the lambda after 500ms;
    }
    TEST_CASE("What De-Register is made-up of")
    {
        CHECK(2 == 3);
    }
}
