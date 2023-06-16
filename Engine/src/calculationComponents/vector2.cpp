#include "vector2.h"

s2d::Vector2::Vector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

s2d::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

s2d::Vector2::Vector2(const ImVec2& vec)
{
	this->x = vec.x;
	this->y = vec.y;
}

bool s2d::Vector2::operator==(const Vector2& rhs)
{
	return(rhs.x == this->x
		&& rhs.y == this->y);
}

bool s2d::Vector2::operator!=(const Vector2& rhs)
{
	return(rhs.x != this->x
		|| rhs.y != this->y);
}

s2d::Vector2 s2d::Vector2::operator+=(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x += rhs.x, this->y += rhs.y);
}

s2d::Vector2 s2d::Vector2::operator*(const float& rhs)
{
	return s2d::Vector2(this->x + rhs, this->y + rhs);
}

s2d::Vector2 s2d::Vector2::operator-(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x - rhs.x, this->y - rhs.y);
}

s2d::Vector2 s2d::Vector2::operator+(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x + rhs.x, this->y + rhs.y);
}

s2d::Vector2 s2d::Vector2::operator*=(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x *= rhs.x, this->y *= rhs.y);
}

s2d::Vector2 s2d::Vector2::operator/=(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x /= rhs.x, this->y /= rhs.y);
}

s2d::Vector2 s2d::Vector2::operator/=(const float rhs)
{
	return s2d::Vector2(this->x /= rhs, this->y /= rhs);
}
s2d::Vector2 s2d::Vector2::operator*(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x * rhs.x, this->y * rhs.y);
}

ImVec2 s2d::Vector2::toImVec2(const s2d::Vector2& vec)
{
	return ImVec2(vec.x, vec.y);
}


std::ostream& s2d::operator<<(std::ostream& os, const s2d::Vector2& rhs)
{
	os << rhs.x << " x " << rhs.y << " y ";
	return os;
}

const s2d::Vector2 s2d::Vector2::SCREEN_MIDDLE = s2d::Vector2(1920 / 2, 1080 / 2);