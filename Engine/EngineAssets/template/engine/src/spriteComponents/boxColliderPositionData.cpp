#include "boxColliderPositionData.h"

s2d::BoxColliderPositionData::BoxColliderPositionData()
{
	resetPosition();
}

void s2d::BoxColliderPositionData::resetPosition()
{
	for (int i = 0; i < s2d::BoxColliderPositionData::s_canCollideSpritesAround; i++)
	{
		this->position[i] = s2d::BoxColliderPositionData::Position::Unknown;
	}
}

bool s2d::BoxColliderPositionData::isEverythingUnknown() const
{
 	int cnt = 0;
	for (int i = 0; i < s2d::BoxColliderPositionData::s_canCollideSpritesAround; i++)
	{
		if (this->position[i] == s2d::BoxColliderPositionData::Position::Unknown)
		{
			cnt++;
		}
	}
	return cnt == s2d::BoxColliderPositionData::s_canCollideSpritesAround;
}

bool s2d::BoxColliderPositionData::isEqual(s2d::BoxColliderPositionData::Position p) const
{
	for (int i = 0; i < s2d::BoxColliderPositionData::s_canCollideSpritesAround; i++)
	{
		if (position[i] == p)
		{
			return true;
		}
	}
	return false;
}

int s2d::BoxColliderPositionData::s_canCollideSpritesAround = 255 * 2;
