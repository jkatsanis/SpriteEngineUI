#include "UITransform.h"

void spe::UITransform::SetLastPosition()
{
	if (this->NextPosition != this->Position)
	{
		this->LastPosition = this->NextPosition;
		this->NextPosition = this->Position;
		this->PositionChanged = true;
	}
	else
	{
		this->PositionChanged = false;
	}
}
