#include "vector3.h"


spe::Vector3::Vector3()
	: X(0), Y(0), Z(0) { }


spe::Vector3::Vector3(float x, float y, float z)
	: X(x) , Y(y), Z(z) { }


sf::Vector3f spe::Vector3::ToSFVector3(const spe::Vector3& vec)
{
	return sf::Vector3f(vec.X, vec.Y, vec.Z);
}
