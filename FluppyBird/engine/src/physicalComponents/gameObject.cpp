#include "gameObject.h"

float s2d::GameObject::ssizeMultipliyer = 1.0f;
std::vector <sf::RectangleShape> s2d::GameObject::rects(0);
s2d::Camera s2d::GameObject::camera;