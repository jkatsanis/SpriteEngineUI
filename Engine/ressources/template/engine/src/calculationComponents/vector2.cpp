#include "calculationComponents/vector2.h"

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

s2d::Vector2 s2d::Vector2::operator-=(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x -= rhs.x, this->y -= rhs.y);
}

s2d::Vector2 s2d::Vector2::operator*(const float& rhs)
{
	return s2d::Vector2(this->x + rhs, this->y + rhs);
}

s2d::Vector2 s2d::Vector2::operator-(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x - rhs.x, this->y - rhs.y);
}

s2d::Vector2 s2d::Vector2::operator*=(const float& rhs)
{
	return s2d::Vector2(this->x *= rhs, this->y *= rhs);
}

s2d::Vector2 s2d::Vector2::operator*=(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x *= rhs.x, this->y *= rhs.y);
}

s2d::Vector2 s2d::Vector2::operator/=(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x /= rhs.x, this->y /= rhs.y);
}

s2d::Vector2 s2d::Vector2::operator*(const s2d::Vector2& rhs)
{
	return s2d::Vector2(this->x * rhs.x, this->y * rhs.y);
}

std::ostream& s2d::operator<<(std::ostream& os, const s2d::Vector2& rhs)
{
	os << rhs.x << " x " << rhs.y << " y ";
	return os;
}
