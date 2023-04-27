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
    this->base_component = false;
    this->ptr_sprite = nullptr;
    this->can_collide = false;
    this->is_solid = false;
}
 
// Public functions

void s2d::BoxCollider::reset() 
{
    this->exist = false;
    this->is_solid = false;
    this->can_collide = false;
    this->box_collider_height = Vector2(0, 0);
    this->box_collider_width = Vector2(0, 0);
}
