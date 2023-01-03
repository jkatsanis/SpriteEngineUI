#include "physics.h"

//Public static functions

void s2d::Physics::update()
{
	for (s2d::Sprite* const ptr_activeSprite : s2d::Sprite::activeSprites)
	{
		ptr_activeSprite->physicsBody.fixedUpdate();
	}	
}
