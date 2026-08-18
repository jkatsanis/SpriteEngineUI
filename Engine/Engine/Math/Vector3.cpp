#include "Vector3.h"


spe::Vector3::Vector3()
	: X(0), Y(0), Z(0) { }


spe::Vector3::Vector3(float x, float y, float z)
	: X(x) , Y(y), Z(z) { }


sf::Vector3f spe::Vector3::ToSFVector3(const spe::Vector3& vec)
{
	return sf::Vector3f(vec.X, vec.Y, vec.Z);
}

bool spe::Vector3::operator==(const Vector3& other) const
{
    return X == other.X && Y == other.Y && Z == other.Z;
}

bool spe::Vector3::operator!=(const Vector3& other) const
{
    return !(*this == other);
}

spe::Vector3 spe::Vector3::operator+(const Vector3& other) const
{
    return Vector3(X + other.X, Y + other.Y, Z + other.Z);
}

spe::Vector3 spe::Vector3::operator-(const Vector3& other) const
{
    return Vector3(X - other.X, Y - other.Y, Z - other.Z);
}

spe::Vector3 spe::Vector3::operator*(float scalar) const
{
    return Vector3(X * scalar, Y * scalar, Z * scalar);
}

float spe::Vector3::Distance(const Vector3& other) const
{
    float dx = X - other.X;
    float dy = Y - other.Y;
    float dz = Z - other.Z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
