#include <algorithm>
#include <iostream>
#include <numeric>
#include <thread>

#include "solution.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

static constexpr bool SKIP{false};

TEST_CASE("CInShd allow registration of timed callbacks" * doctest::skip(SKIP) *
          doctest::may_fail()) {
  using namespace std::chrono_literals;
  auto period = 500ms;
  auto tick = 50ms;
  Instant registeredAt, calledAt;
  auto cIn =
      CallbackInfrastructureBuilder().anInstance().WithDefaultTick().Build();

  cIn.registerCallback(period, [&calledAt]() { calledAt = Now(); });
  registeredAt = Now();

  std::this_thread::sleep_for(period + tick + tick);
  auto latency = calledAt - registeredAt;
  CHECK(latency <= (period + tick));
  CHECK(latency >= (period - tick));
}

TEST_CASE(
    "CInShd allow registration and deregistration. Call repeatedly until "
    "deregistration" *
    doctest::skip(SKIP) * doctest::may_fail()) {
  using namespace std::chrono_literals;
  auto period = 500ms;
  auto tick = 50ms;
  unsigned multiple = 3;
  Instant registeredAt;
  std::vector<Instant> snapshots{};
  snapshots.reserve(multiple);
  auto cIn =
      CallbackInfrastructureBuilder().anInstance().WithDefaultTick().Build();

  auto callbackId = cIn.registerCallback(
      period, [&snapshots]() { snapshots.push_back(Now()); });
  registeredAt = Now();
  std::this_thread::sleep_for(multiple * period + tick);
  cIn.deregisterCallback(callbackId);
  auto sizeAfterDereg = snapshots.size();
  std::this_thread::sleep_for(multiple * period + tick);

  CHECK(sizeAfterDereg == multiple);
  Duration minLatency, maxLatency;
  std::accumulate(snapshots.begin(), snapshots.end(), registeredAt,
                  [&minLatency, &maxLatency](auto &previous, auto &current) {
                    auto latency = current - previous;
                    minLatency = std::min(minLatency, latency);
                    maxLatency = std::max(maxLatency, latency);
                    return current;
                  });
  CHECK(sizeAfterDereg == snapshots.size());
  CHECK(minLatency >= (period - tick));
  CHECK(maxLatency <= (period + tick));
}

TEST_CASE("CInShd start calling after registration" * doctest::skip(SKIP) *
          doctest::may_fail()) {}
TEST_CASE("CInShd stop calling after de-registration" * doctest::skip(SKIP) *
          doctest::may_fail()) {}
TEST_CASE(
    "CInShd shall call within period +/- clock-tick for various clock-ticks" *
    doctest::skip(SKIP) * doctest::may_fail()) {}
TEST_CASE("CInShd disallow registration when period is not multiple of tick" *
          doctest::skip(SKIP) * doctest::may_fail()) {}
