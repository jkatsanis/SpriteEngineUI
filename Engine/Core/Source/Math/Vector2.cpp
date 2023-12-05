#include "Vector2.h"

spe::Vector2::Vector2()
{
	this->X = 0.0f;
	this->Y = 0.0f;
}

spe::Vector2::Vector2(float x, float y)
{
	this->X = x;
	this->Y = y;
}

spe::Vector2::Vector2(const ImVec2& vec)
{
	this->X = vec.x;
	this->Y = vec.y;
}

spe::Vector2::Vector2(const spe::Vector2& rhs)
{
	this->X = rhs.X;
	this->Y = rhs.Y;
}

spe::Vector2::Vector2(const sf::Vector2f& rhs)
{
	this->X = rhs.x;
	this->Y = rhs.y;
}

bool spe::Vector2::operator==(const Vector2& rhs) const
{
	return(rhs.X == this->X
		&& rhs.Y == this->Y);
}

bool spe::Vector2::operator!=(const Vector2& rhs) const
{
	return(rhs.X != this->X
		|| rhs.Y != this->Y);
}

spe::Vector2 spe::Vector2::operator+=(const spe::Vector2& rhs)
{
	return spe::Vector2(this->X += rhs.X, this->Y += rhs.Y);
}

spe::Vector2 spe::Vector2::operator*(const float& rhs) const
{
	return spe::Vector2(this->X * rhs, this->Y * rhs);
}
 
spe::Vector2 spe::Vector2::operator-(const spe::Vector2& rhs) const
{
	return spe::Vector2(this->X - rhs.X, this->Y - rhs.Y);
}

spe::Vector2 spe::Vector2::operator+(const spe::Vector2& rhs) const
{
	return spe::Vector2(this->X + rhs.X, this->Y + rhs.Y);
}

spe::Vector2 spe::Vector2::operator*=(const spe::Vector2& rhs)
{
	return spe::Vector2(this->X *= rhs.X, this->Y *= rhs.Y);
}

spe::Vector2 spe::Vector2::operator/=(const spe::Vector2& rhs) 
{
	return spe::Vector2(this->X /= rhs.X, this->Y /= rhs.Y);
}

spe::Vector2 spe::Vector2::operator/=(const float rhs)
{
	return spe::Vector2(this->X /= rhs, this->Y /= rhs);
}
spe::Vector2 spe::Vector2::operator*(const spe::Vector2& rhs) const
{
	return spe::Vector2(this->X * rhs.X, this->Y * rhs.Y);
}

ImVec2 spe::Vector2::toImVec2(const spe::Vector2& vec)
{
	return ImVec2(vec.X, vec.Y);
}

sf::Vector2f spe::Vector2::toSFVector(const spe::Vector2& vec)
{
	return sf::Vector2f(vec.X, vec.Y);
}


std::ostream& spe::operator<<(std::ostream& os, const spe::Vector2& rhs)
{
	os << rhs.X << " x " << rhs.Y << " y ";
	return os;
}

const spe::Vector2 spe::Vector2::SCREEN_MIDDLE = spe::Vector2(1920 / 2, 1080 / 2);