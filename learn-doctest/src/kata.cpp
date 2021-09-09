#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

int factorial(int number) {
    return number <= 1 ? 1 : number * factorial(number - 1);
}

TEST_CASE("Test factorial")
{
    SUBCASE("Factorial of 0 is 1")
    {
        CHECK(factorial(0) == 1);
    }
    SUBCASE("Factorial of 1 is 1")
    {
        CHECK(factorial(1) == 1);
    }
    SUBCASE("Factorial of 2 is 2")
    {
        CHECK(factorial(2) == 2);
    }
    SUBCASE("Factorial of 3 is 6")
    {
        CHECK(factorial(3) == 6);
    }
    SUBCASE("Factorial of 10 is 3628800")
    {
        CHECK(factorial(10) == 3628800);
    }
}


