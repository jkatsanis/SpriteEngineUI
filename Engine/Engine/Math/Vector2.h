#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include <cmath> // <-- for std::sqrt

namespace spe
{
	class Vector2
	{
	public:
		float X, Y;

		Vector2();
		Vector2(float x, float y);
		Vector2(const ImVec2& vec);
		Vector2(const spe::Vector2& rhs);
		Vector2(const sf::Vector2f& rhs);

		bool operator==(const Vector2& rhs) const;
		bool operator!=(const Vector2& rhs) const;
		spe::Vector2 operator+=(const spe::Vector2& rhs);
		spe::Vector2 operator*(const float& rhs) const;
		spe::Vector2 operator-(const spe::Vector2& rhs) const;
		spe::Vector2 operator+(const spe::Vector2& rhs) const;
		spe::Vector2 operator*=(const spe::Vector2& rhs);
		spe::Vector2 operator/=(const spe::Vector2& rhs);
		spe::Vector2 operator/=(const float rhs);
		spe::Vector2 operator*(const spe::Vector2& rhs) const;

		friend std::ostream& operator<<(std::ostream& os, const spe::Vector2& rhs);

		float Length() const {
			return std::sqrt(X * X + Y * Y);
		}

		void Normalize();

	public:
		static Vector2 GetDirection(const Vector2& A, const Vector2& B);

		static const spe::Vector2 SCREEN_MIDDLE;
		static ImVec2 toImVec2(const spe::Vector2& vec);
		static sf::Vector2f toSFVector(const spe::Vector2& vec);
	};

	std::ostream& operator<<(std::ostream& os, const spe::Vector2& rhs);
}