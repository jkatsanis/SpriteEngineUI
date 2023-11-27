#pragma once

#include <Include/SFML/Graphics.hpp>

namespace spe
{
	class Vector3
	{
	public:
		float x, y, z;

		Vector3();
		Vector3(float x, float y, float z);

		static sf::Vector3f toSFVector3(const spe::Vector3& rhs);		
	};
}

