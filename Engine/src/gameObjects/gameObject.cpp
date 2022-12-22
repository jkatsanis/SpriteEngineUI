#include "gameObject.h"

std::vector<s2d::Line> s2d::GameObject::lines(0);
std::vector <sf::RectangleShape> s2d::GameObject::rects(0);
s2d::Camera* s2d::GameObject::ptr_camera_tRealTimeEditor = nullptr;
