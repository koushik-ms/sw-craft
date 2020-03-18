#include <iostream>
#include <chrono>
#include <thread>
#include <numeric>

class CallbackInfrastructure {
    public:
    template<typename A, typename B>
    int registerCallback(A duration, B callback) { return 0; };
    void deregisterCallback(int id) {};
};

using Instant = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::system_clock::duration;
auto Now() { return std::chrono::system_clock::now(); }


//---
#include <algorithm>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
TEST_CASE("CInShd allow registration of timed callbacks") {
    using namespace std::chrono_literals;
    auto period = 500ms;
    auto tick = 50ms;
    Instant registeredAt, calledAt;
    CallbackInfrastructure cIn{};

    cIn.registerCallback(period, [&calledAt]() { calledAt = Now(); });
    registeredAt = Now();

    std::this_thread::sleep_for(period+tick+tick);
    auto latency = calledAt - registeredAt;
    CHECK(latency  <= (period+tick));
    CHECK(latency  >= (period-tick));
}

TEST_CASE("CInShd allow registration and deregistration. Call repeatedly until deregistration") {
    using namespace std::chrono_literals;
    auto period = 500ms;
    auto tick = 50ms;
    unsigned multiple = 3;
    Instant registeredAt;
    std::vector<Instant> snapshots{};
    snapshots.reserve(multiple);
    CallbackInfrastructure cIn{};

    auto callbackId = cIn.registerCallback(period, [&snapshots]() { 
        snapshots.push_back(Now()); 
        } );
    registeredAt = Now();
    std::this_thread::sleep_for(multiple*period + tick);
    cIn.deregisterCallback(callbackId);
    auto sizeAfterDereg = snapshots.size();
    std::this_thread::sleep_for(multiple*period+tick);

    CHECK(sizeAfterDereg == multiple);
    Duration minLatency, maxLatency;
    std::accumulate(
        snapshots.begin(), snapshots.end(), 
        registeredAt, 
        [&minLatency, &maxLatency](auto &previous, auto &current) {
            auto latency = current - previous;
            minLatency = std::min(minLatency, latency);
            maxLatency = std::max(maxLatency, latency);
            return current;
        }
    );
    CHECK(sizeAfterDereg == snapshots.size());
    CHECK(minLatency >= (period-tick));
    CHECK(maxLatency <= (period+tick));
}
