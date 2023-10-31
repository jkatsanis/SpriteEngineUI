#pragma once

#include <SFML/Graphics.hpp>

namespace s2d
{
	class Vector3
	{
	public:
		float x, y, z;

		Vector3();
		Vector3(float x, float y, float z);

		static sf::Vector3f toSFVector3(const s2d::Vector3& rhs);
	};
}

