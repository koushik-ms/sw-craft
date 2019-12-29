#include <iostream>
#include <vector>

/*
    Prime factors kata
*/
using std::vector;
// @todo: isDivisible doesn't read well. Doesn't convey intent.
bool isDivisible(unsigned dividend, unsigned divisor) { return (dividend % divisor) == 0; }

vector<unsigned> primeFactors(unsigned number) {
    std::vector<unsigned> result{};
    unsigned limit = number / 2;
    for(unsigned int factor = 2; factor <= limit; ++factor) {
        if(isDivisible(number, factor)) result.push_back(factor);
        while(isDivisible(number, factor)) number /= factor;
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
    CHECK(primeFactors(1) ==  std::vector<unsigned>());
    CHECK(primeFactors(2) ==  std::vector<unsigned>());
    CHECK(primeFactors(4) ==  std::vector<unsigned>{{2}});
    CHECK(primeFactors(5) ==  std::vector<unsigned>());
    CHECK(primeFactors(6) ==  std::vector<unsigned>{{2, 3}});
    CHECK(primeFactors(8) ==  std::vector<unsigned>{{2}});
    CHECK(primeFactors(9) ==  std::vector<unsigned>{{3}});
}

TEST_CASE("Prime Factors two digit numbers") {
    CHECK(primeFactors(10) ==  std::vector<unsigned>{{2, 5}});
    CHECK(primeFactors(18) ==  std::vector<unsigned>{{2, 3}});
    CHECK(primeFactors(27) ==  std::vector<unsigned>{{3}});
    CHECK(primeFactors(30) ==  std::vector<unsigned>{{2, 3, 5}});
    CHECK(primeFactors(76) ==  std::vector<unsigned>{{2, 19}});
}

TEST_CASE("Prime Factors larger numbers") {
    CHECK(primeFactors( 128) == std::vector<unsigned>{{2}});
    CHECK(primeFactors( 210) == std::vector<unsigned>{{2, 3, 5, 7}});
    CHECK(primeFactors(2310) == std::vector<unsigned>{{2, 3, 5, 7, 11}});
}

TEST_CASE("Test kata" * doctest::skip(true)) {
    vector<int> v{1, 2, 3};
    vector<int> x = {1, 2, 3, 4};
    std::cout << v << std::endl;
    std::cout << x << std::endl;
    CHECK(v == x); // @todo: stringification is not working. needs fix.
}
