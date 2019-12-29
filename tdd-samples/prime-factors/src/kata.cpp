#include <vector>
#include <iostream>

/*
    Prime factors kata
*/
using std::vector;
// @todo: isDivisible doesn't read well. Doesn't convey intent.
bool isDivisible(unsigned dividend, unsigned divisor) { return (dividend % divisor) == 0; }

vector<unsigned> primeFactors(unsigned number) {
    std::vector<unsigned> result{};
    unsigned limit = number / 2;
    for(unsigned const& factor: {2, 3, 5}) {
        if(factor > limit) break;
        if(isDivisible(number, factor)) result.push_back(factor);
    }
    return result;
}

template <typename T>
std::ostream& operator<< (std::ostream &os, vector<T> const& vec) {
    os << '{';
    for(auto const& elem: vec) {
        os << elem << ' ';
    }
    os << '}';
    return os;
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Prime factors single digit numbers") {
    SUBCASE("Prime Factors of 1 is empty list") {
        auto x = primeFactors(1);
        auto expected = std::vector<unsigned>();
        CHECK(x == expected);
    }
    SUBCASE("Prime Factors of 2 is empty list") {
        auto x = primeFactors(2);
        auto expected = std::vector<unsigned>();
        CHECK(x == expected);
    }
    SUBCASE("Prime Factors of 4 is {2}") {
        auto expected = std::vector<unsigned>{{2}};
        auto x = primeFactors(4);
        CHECK(x == expected);
    }
    SUBCASE("Prime Factors of 5 is empty list") {
        auto x = primeFactors(5);
        auto expected = std::vector<unsigned>();
        CHECK(x == expected);
    }
    SUBCASE("Prime Factors of 6 is {2, 3}") {
        auto expected = std::vector<unsigned>{{2, 3}};
        auto x = primeFactors(6);
        CHECK(x == expected);
    }
    SUBCASE("Prime Factors of 9 is {3}") {
        auto expected = std::vector<unsigned>{{3}};
        auto x = primeFactors(9);
        CHECK(x == expected);
    }
}

TEST_CASE("Prime Factors two digit numbers") {
    SUBCASE("Prime factors of 10 is {2, 5}") {
        auto expected = std::vector<unsigned>{{2, 5}};
        auto x = primeFactors(10);
        std::cout << expected << std::endl;
        std::cout << x << std::endl;
        CHECK(x == expected);
    }
}

TEST_CASE("Test kata" * doctest::skip(true)) {
    CHECK(true);
    vector<int> v{1, 2, 3};
    vector<int> x = {1, 2, 3, 4};
    std::cout << v << std::endl;
    std::cout << x << std::endl;
    CHECK(v == x); // @todo: stringification is not working. needs fix.
}
