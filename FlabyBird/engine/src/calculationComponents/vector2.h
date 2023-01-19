#pragma once

#include <ostream>

namespace s2d
{
	class Vector2
	{
	public:
		float x, y;

		Vector2();
		Vector2(float x, float y);

		bool operator==(const Vector2& rhs);
		bool operator!=(const Vector2& rhs);
		s2d::Vector2 operator+=(const s2d::Vector2& rhs);
		s2d::Vector2 operator-=(const s2d::Vector2& rhs);
		s2d::Vector2 operator*(const float& rhs);
		s2d::Vector2 operator-(const s2d::Vector2& rhs);
		s2d::Vector2 operator*=(const float& rhs);
		s2d::Vector2 operator*=(const s2d::Vector2& rhs);
		s2d::Vector2 operator/=(const s2d::Vector2& rhs);
		s2d::Vector2 operator*(const s2d::Vector2& rhs);

		friend std::ostream& operator<<(std::ostream& os, const s2d::Vector2& rhs);
	};

	std::ostream& operator<<(std::ostream& os, const s2d::Vector2& rhs);
}

