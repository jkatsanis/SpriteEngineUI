#include "GameUtils.h"

bool spe::GameUtils::IsLeft(spe::Sprite* maybe_left, spe::Sprite* maybe_right)
{
    // Get the X coordinates of both sprites
    float x_left = maybe_left->Transform.GetPosition().X;
    float x_right = maybe_right->Transform.GetPosition().X;

    // Return true if maybe_left is to the left of maybe_right
    return x_left < x_right;
}
