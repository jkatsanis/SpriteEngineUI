#pragma once

#include <ostream>
#include <SFML/Graphics.hpp>
#include <ImGui.h>

namespace s2d
{
	class Vector2
	{
	public:
		float x, y;

		Vector2();
		Vector2(float x, float y);
		Vector2(const s2d::Vector2& rhs);

		bool operator==(const Vector2& rhs);
		bool operator!=(const Vector2& rhs);
		s2d::Vector2 operator+=(const s2d::Vector2& rhs);
		s2d::Vector2 operator-=(const s2d::Vector2& rhs);
		s2d::Vector2 operator*(const float& rhs);
		s2d::Vector2 operator*=(const float& rhs);
		s2d::Vector2 operator*=(const s2d::Vector2& rhs);
		s2d::Vector2 operator/=(const s2d::Vector2& rhs);
		s2d::Vector2 operator*(const s2d::Vector2& rhs);
		s2d::Vector2 operator+(const s2d::Vector2& rhs);
		s2d::Vector2 operator-(const s2d::Vector2& rhs) const;

		static s2d::Vector2 distanceBetween(const s2d::Vector2& lhs, const s2d::Vector2& rhs);

		friend std::ostream& operator<<(std::ostream& os, const s2d::Vector2& rhs);

	public:
		static ImVec2 toImVec2(const s2d::Vector2& vec);
		static sf::Vector2f toSFVector(const s2d::Vector2& vec);
	};

	std::ostream& operator<<(std::ostream& os, const s2d::Vector2& rhs);
}

