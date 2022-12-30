#include "boxCollider.h"
#include <sprite.h>

s2d::BoxCollider::BoxCollider()
	: isSolid(false), canCollide(false), exists(false)
{
    this->ptr_sprite = nullptr;
}

s2d::BoxCollider::BoxCollider(s2d::Sprite* ptr_sprite)
	: exists(false)
{
    this->ptr_sprite = ptr_sprite;
    this->canCollide = false;
    this->isSolid = false;
}
 

void s2d::BoxCollider::resetBoxCollider()
{
    this->exists = false;
    this->isSolid = false;
    this->canCollide = false;
    this->boxColliderHeightUpOrDown = Vector2(0, 0);
    this->boxColliderWidthLeftOrRight = Vector2(0, 0);
}
