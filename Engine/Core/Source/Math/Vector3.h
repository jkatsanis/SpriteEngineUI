#pragma once

#include <Include/SFML/Graphics.hpp>

namespace spe
{
	class Vector3
	{
	public:
		float X, Y, Z;

		Vector3();
		Vector3(float x, float y, float z);

		static sf::Vector3f ToSFVector3(const spe::Vector3& rhs);		
	};
}

