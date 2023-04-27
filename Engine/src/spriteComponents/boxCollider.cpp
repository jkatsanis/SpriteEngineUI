#include "boxCollider.h"
#include <physicalComponents/sprite.h>

// Constructor / Destructor

s2d::BoxCollider::BoxCollider()
{
    this->ptr_sprite = nullptr;
}

s2d::BoxCollider::BoxCollider(s2d::Sprite* ptr_sprite)
{
    this->init();
    this->ptr_sprite = ptr_sprite;
}
void s2d::BoxCollider::init()
{
    this->ptr_sprite = nullptr;
    this->canCollide = false;
    this->isSolid = false;
}
 
// Public functions

void s2d::BoxCollider::reset() 
{
    this->exist = false;
    this->isSolid = false;
    this->canCollide = false;
    this->boxColliderHeightUpOrDown = Vector2(0, 0);
    this->boxColliderWidthLeftOrRight = Vector2(0, 0);
}
