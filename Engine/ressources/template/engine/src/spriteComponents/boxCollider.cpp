#include "boxCollider.h"
#include <physicalComponents/sprite.h>

// Constructor / Destructor 

s2d::BoxCollider::BoxCollider()
{
    this->init();
}

s2d::BoxCollider::BoxCollider(s2d::Sprite* sprite)
{
    this->init();

    this->ptr_attached_sprite = sprite;
}

s2d::BoxCollider::BoxCollider(s2d::Sprite* sprite, s2d::BoxCollider& rhs)
{
    this->init();
    this->resetPositions();
    this->ptr_attached_sprite = sprite;
    this->box_collider_height = rhs.box_collider_height;
    this->box_collider_width = rhs.box_collider_width;
    this->exist = rhs.exist;
    this->is_solid = rhs.is_solid;
}


void s2d::BoxCollider::init()
{
    this->is_solid = false;
    this->exist = false;
    this->ptr_attached_sprite = nullptr;
    this->colliding_sprite = nullptr;
    this->can_collide = false;
}


// Public methods


void s2d::BoxCollider::reset()
{
    this->exist = false;
    this->is_solid = false;
    this->can_collide = false;
    this->box_collider_height = Vector2(0, 0);
    this->box_collider_width = Vector2(0, 0);
}

void s2d::BoxCollider::resetPositions()
{
    this->colliding_sprite = nullptr;
    this->collided = false;
    this->right = false;
    this->left = false;
    this->down = false;
    this->up = false;
}

//Private functions

void s2d::BoxCollider::checkPositions(const BoxCollider& other)
{
    short range = 10;

    // Right
    if (this->ptr_attached_sprite->getOrigininalPosition().x + this->ptr_attached_sprite->transform.texture_size.x + this->ptr_attached_sprite->collider.box_collider_width.y >= other.ptr_attached_sprite->getOrigininalPosition().x + other.box_collider_width.x
        && this->ptr_attached_sprite->getOrigininalPosition().x + this->ptr_attached_sprite->transform.texture_size.x + this->ptr_attached_sprite->collider.box_collider_width.y <= other.ptr_attached_sprite->getOrigininalPosition().x + other.box_collider_width.x + range)
    {
        this->right = true;
        return;
    }

    // Left

    if (this->ptr_attached_sprite->getOrigininalPosition().x + this->box_collider_width.x <= other.ptr_attached_sprite->getOrigininalPosition().x + other.ptr_attached_sprite->transform.texture_size.x + other.box_collider_width.y
        && this->ptr_attached_sprite->getOrigininalPosition().x + this->box_collider_width.x + range >= other.ptr_attached_sprite->getOrigininalPosition().x + other.ptr_attached_sprite->transform.texture_size.x + other.box_collider_width.y)
    {
        this->left = true;
        return;
    }

    // Down

    if (this->ptr_attached_sprite->getOrigininalPosition().y + this->ptr_attached_sprite->transform.texture_size.y + this->ptr_attached_sprite->collider.box_collider_height.y >= other.ptr_attached_sprite->getOrigininalPosition().y + other.box_collider_height.x
        && (this->ptr_attached_sprite->getOrigininalPosition().y + this->ptr_attached_sprite->transform.texture_size.y + this->ptr_attached_sprite->collider.box_collider_height.y <= other.ptr_attached_sprite->getOrigininalPosition().y + other.box_collider_height.x + range))
    {
        this->down = true;   
        return;
    }

    this->up = true;
    return;
}

//Static functions

void s2d::BoxCollider::checkCollisions(s2d::SpriteRepository& repo)
{
    for (size_t i = 0; i < repo.amount(); i++)
    {
        s2d::Sprite* ds = repo.readAt(i);
        ds->collider.collided_in_frame = false;
    }
    for (size_t i = 0; i < repo.amount(); i++)
    {
        s2d::Sprite* i_s = repo.readAt(i);

        for (size_t j = 0; j < repo.amount(); j++)
        {
            s2d::Sprite* j_s = repo.readAt(j);

            if (i_s->getId() == j_s->getId())
            {
                continue;
            }

            if (s2d::BoxCollider::checkCollision(i_s->collider, j_s->collider))
            {
                i_s->collider.colliding_sprite = j_s;
                j_s->collider.colliding_sprite = i_s;
            }
        }
    }

    // set the colliding sprite to nulltpr if the sprite doens't collide
    for (int i = 0; i < repo.amount(); i++)
    {
        s2d::Sprite* const sprite = repo.readAt(i);
        if (!sprite->collider.collided_in_frame)
        {
            sprite->collider.resetPositions();
        }
    }
}


bool s2d::BoxCollider::checkCollision(s2d::BoxCollider& other, s2d::BoxCollider& rhs)
{
    s2d::Sprite* ptr_attached_sprite = rhs.ptr_attached_sprite;
    if (!rhs.exist || !other.exist)
    {
        return false;
    }

    float getPosX = ptr_attached_sprite->getOrigininalPosition().x;
    float getPosY = ptr_attached_sprite->getOrigininalPosition().y;

    float otherGetPosX = other.ptr_attached_sprite->getOrigininalPosition().x;
    float otherGetPosY = other.ptr_attached_sprite->getOrigininalPosition().y;

    if (getPosX + ptr_attached_sprite->transform.texture_size.x + rhs.box_collider_width.y >= otherGetPosX + other.box_collider_width.x
        && getPosX + rhs.box_collider_width.x <= otherGetPosX + other.box_collider_width.y + other.ptr_attached_sprite->transform.texture_size.x
        && getPosY + ptr_attached_sprite->transform.texture_size.y + rhs.box_collider_height.y >= otherGetPosY + other.box_collider_height.x
        && getPosY + rhs.box_collider_height.x <= otherGetPosY + other.box_collider_height.y + other.ptr_attached_sprite->transform.texture_size.y)
    {
        rhs.resetPositions();
        other.resetPositions();

        other.collided = true;
        other.collided_in_frame = true;
        rhs.collided = true;
        rhs.collided_in_frame = true;

        rhs.checkPositions(other);
        other.checkPositions(rhs);

        return true;
    }
    return false;
}