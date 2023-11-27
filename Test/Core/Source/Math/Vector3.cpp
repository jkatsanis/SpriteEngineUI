#include "vector3.h"


spe::Vector3::Vector3()
	: x(0), y(0), z(0) { }


spe::Vector3::Vector3(float x, float y, float z)
	: x(x) , y(y), z(z) { }


sf::Vector3f spe::Vector3::toSFVector3(const spe::Vector3& vec)
{
	return sf::Vector3f(vec.x, vec.y, vec.z);
}
