#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

/*
Below are the rules of the bowling game:
1. A bowling game has 10 frames, each with max 2 throws.
2. Each throw can have a score from 0 to 10 (bot incl) based on number of pins knocked down in that throw. 0 is a gutter ball and 10 represents a "strike".
3. A spare is when all 10 pins are knocked down in the 2 throws of the same frame.
4. A spare is scored by adding 10 to the number of pins knocked in the following throw.
5. A strike is when all 10 pins are knocked down in 1 throw. In this case the frame has only one throw.
6. A spare is scored by adding 10 to the number of pins knocked down in the 2 subsequent throws (which could also be upto 10).
*/

// High-level design
// A game class has some frame objects (10 of them) which contain refs to next frame and their throws.
// To calculate score of the game calculate score of each frame and add up. Score of frame depends on score of throws.

class Game { 
    public:
    void roll(unsigned pinsDown) {
        throws[currentThrow++] = pinsDown;
     }
    unsigned score() { 
        unsigned score{0}; 
        unsigned firstInFrame{0};
        for(unsigned frame = 0; frame < 10; ++frame) {
            if(isStrike(firstInFrame)) {
                score += 10 + nextTwoThrows(firstInFrame);
                firstInFrame += 1;
            } else if(isSpare(firstInFrame)) {
                score += 10 + throwAfterSpare(firstInFrame);
                firstInFrame += 2;
            } else {
                score += frameTotal(firstInFrame);
                firstInFrame += 2;
            }
        }
        return score; 
    }
    private:
    bool isSpare(unsigned index) { return throws[index] + throws[index+1] == 10; }
    bool isStrike(unsigned index) { return throws[index] == 10; }
    unsigned nextTwoThrows(unsigned index) { return throws[index+1] + throws[index+2]; }
    unsigned frameTotal(unsigned index) { return throws[index] + throws[index+1]; }
    unsigned throwAfterSpare(unsigned index) { return throws[index+2]; }
    std::array<unsigned, 21> throws{};
    unsigned currentThrow{0};
};

class GameTest {
    protected:
    void roll(unsigned times, unsigned pins) {
        for(unsigned i = 0; i < times; ++i) {
            g.roll(pins);
        }
    }
    void rollSpare() { g.roll(5); g.roll(5); }
    void rollStrike() { g.roll(10); }
    Game g;
};

TEST_CASE_FIXTURE(GameTest, "Bowling game kata") {
    SUBCASE("Can score a gutter game") {
        roll(20U, 0U);
        CHECK(0 == g.score());
    }
    SUBCASE("Can score a game of ones") {
        roll(20, 1);
        CHECK(20 == g.score());
    }
    SUBCASE("Can score a game of threes") {
        roll(20, 3);
        CHECK(60 == g.score());
    }
}

TEST_CASE_FIXTURE(GameTest, "Bowling game kata") {
    SUBCASE("Can score a game with a spare and a two") {
        rollSpare();
        roll(1, 2);
        roll(17, 0);
        CHECK(14 == g.score());
    }
    SUBCASE("Can score a game of spares") {
        roll(21, 5);
        CHECK(150 == g.score());
    }
    SUBCASE("Can score strike + 4 + 2") {
        rollStrike();
        roll(1, 4);
        roll(1, 2);
        roll(16, 0);
        CHECK(22 == g.score());
    }
    SUBCASE("Can score a perfect game") {
        roll(12, 10);
        CHECK(300 == g.score());
    }
}

TEST_CASE_FIXTURE(GameTest, "Bowling game kata" * doctest::skip(true)) {
    SUBCASE("Incorrect shd fail") {
        roll(12, 1);
        CHECK(33 == g.score());
    }
}
