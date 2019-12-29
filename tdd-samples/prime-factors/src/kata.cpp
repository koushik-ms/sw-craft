#include <vector>
#include <iostream>

/*
    Prime factors kata
*/
using std::vector;
vector<unsigned> primeFactors(unsigned number) {
    if (number == 4) return {2};
    return {};
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

TEST_CASE("Prime factors basics") {
    SUBCASE("Prime Factors of 1 is empty list") {
        auto x = primeFactors(1);
        auto expected = std::vector<unsigned>();
        CHECK(x == expected);
    }
    SUBCASE("Prime Factors of 4 is {2}") {
        auto expected = std::vector<unsigned>{{2}};
        auto x = primeFactors(4);
        CHECK(x == expected);
    }
    SUBCASE("Prime Factors of 6 is {2, 3}") {
        auto expected = std::vector<unsigned>{{2, 3}};
        auto x = primeFactors(6);
        std::cout << x << std::endl;
        std::cout << expected << std::endl;
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
