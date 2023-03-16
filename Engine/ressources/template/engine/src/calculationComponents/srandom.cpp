#include "srandom.h"

int s2d::Random::getRandomNumber(int from, int to)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(from, to);
    return uni(rng);
}
