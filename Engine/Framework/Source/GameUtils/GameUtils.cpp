#include "GameUtils.h"

bool spe::GameUtils::IsLeft(spe::Sprite* maybe_left, spe::Sprite* maybe_right)
{
    // Get the X coordinates of both sprites
    float x_left = maybe_left->Transform.GetPosition().X;
    float x_right = maybe_right->Transform.GetPosition().X;

    // Return true if maybe_left is to the left of maybe_right
    return x_left < x_right;
}

int32_t spe::GameUtils::Random(int32_t min, int32_t max)
{
    std::random_device rd;

    std::mt19937 gen(rd());

    // Define the distribution range
    std::uniform_int_distribution<> distr(min, max);

    // Generate a random number within the range
    return distr(gen);
}

bool spe::GameUtils::IsInRangeX(const spe::Sprite* sprite1, const spe::Sprite* sprite2, int range)
{
    return std::abs(sprite1->Transform.GetPosition().X - sprite2->Transform.GetPosition().X) <= range;
}
