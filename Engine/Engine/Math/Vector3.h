#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

namespace spe
{
	class Vector3
	{
	public:
		float X, Y, Z;

		Vector3();
		Vector3(float x, float y, float z);

		static sf::Vector3f ToSFVector3(const spe::Vector3& rhs);		

        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;
        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator*(float scalar) const;

        float Distance(const Vector3& other) const;
	};
}
