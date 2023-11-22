#include "Vector2.h"

spe::Vector2::Vector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

spe::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

spe::Vector2::Vector2(const ImVec2& vec)
{
	this->x = vec.x;
	this->y = vec.y;
}

spe::Vector2::Vector2(const spe::Vector2& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
}

spe::Vector2::Vector2(const sf::Vector2f& rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
}

bool spe::Vector2::operator==(const Vector2& rhs)
{
	return(rhs.x == this->x
		&& rhs.y == this->y);
}

bool spe::Vector2::operator!=(const Vector2& rhs)
{
	return(rhs.x != this->x
		|| rhs.y != this->y);
}

spe::Vector2 spe::Vector2::operator+=(const spe::Vector2& rhs)
{
	return spe::Vector2(this->x += rhs.x, this->y += rhs.y);
}

spe::Vector2 spe::Vector2::operator*(const float& rhs)
{
	return spe::Vector2(this->x * rhs, this->y * rhs);
}

spe::Vector2 spe::Vector2::operator-(const spe::Vector2& rhs)
{
	return spe::Vector2(this->x - rhs.x, this->y - rhs.y);
}

spe::Vector2 spe::Vector2::operator+(const spe::Vector2& rhs)
{
	return spe::Vector2(this->x + rhs.x, this->y + rhs.y);
}

spe::Vector2 spe::Vector2::operator*=(const spe::Vector2& rhs)
{
	return spe::Vector2(this->x *= rhs.x, this->y *= rhs.y);
}

spe::Vector2 spe::Vector2::operator/=(const spe::Vector2& rhs)
{
	return spe::Vector2(this->x /= rhs.x, this->y /= rhs.y);
}

spe::Vector2 spe::Vector2::operator/=(const float rhs)
{
	return spe::Vector2(this->x /= rhs, this->y /= rhs);
}
spe::Vector2 spe::Vector2::operator*(const spe::Vector2& rhs)
{
	return spe::Vector2(this->x * rhs.x, this->y * rhs.y);
}

ImVec2 spe::Vector2::toImVec2(const spe::Vector2& vec)
{
	return ImVec2(vec.x, vec.y);
}

sf::Vector2f spe::Vector2::toSFVector(const spe::Vector2& vec)
{
	return sf::Vector2f(vec.x, vec.y);
}


std::ostream& spe::operator<<(std::ostream& os, const spe::Vector2& rhs)
{
	os << rhs.x << " x " << rhs.y << " y ";
	return os;
}

const spe::Vector2 spe::Vector2::SCREEN_MIDDLE = spe::Vector2(1920 / 2, 1080 / 2);