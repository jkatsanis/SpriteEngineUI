#pragma once

#include <iostream>
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

		bool operator==(const Vector2& rhs);
		bool operator!=(const Vector2& rhs);
		s2d::Vector2 operator+=(const s2d::Vector2& rhs);
		s2d::Vector2 operator*(const float& rhs);
		s2d::Vector2 operator-(const s2d::Vector2& rhs);
		s2d::Vector2 operator+(const s2d::Vector2& rhs);
		s2d::Vector2 operator*=(const s2d::Vector2& rhs);
		s2d::Vector2 operator/=(const s2d::Vector2& rhs);
		s2d::Vector2 operator/=(const float rhs);
		s2d::Vector2 operator*(const s2d::Vector2& rhs);

		friend std::ostream& operator<<(std::ostream& os, const s2d::Vector2& rhs);

	public:
		static const s2d::Vector2 SCREEN_MIDDLE;
		static ImVec2 toImVec2(const s2d::Vector2& vec);
	};

	std::ostream& operator<<(std::ostream& os, const s2d::Vector2& rhs);
}

