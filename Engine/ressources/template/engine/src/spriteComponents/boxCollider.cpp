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
    this->collision_cnt = 0;
    this->can_collide = false;
}


// Public methods

bool s2d::BoxCollider::checkCollision(s2d::BoxCollider& other, const int jIndex)
{
    //Already got collision detection working

    if (!this->exist || !other.exist)
    {
        return false;
    }

    //May need to do better sprite pos detection!
    //Using for example this->boxColliderWidthLeftOrRight.y when we getPosX and add sprite size because we can adjust the right widtg in the inspector and the right width is y

    float getPosX = ptr_attached_sprite->getOrigininalPosition().x;
    float getPosY = ptr_attached_sprite->getOrigininalPosition().y;

    float otherGetPosX = other.ptr_attached_sprite->getOrigininalPosition().x;
    float otherGetPosY = other.ptr_attached_sprite->getOrigininalPosition().y;

    if (getPosX + ptr_attached_sprite->transform.texture_size.x + this->box_collider_width.y >= otherGetPosX + other.box_collider_width.x
        && getPosX + this->box_collider_width.x <= otherGetPosX + other.box_collider_width.y + other.ptr_attached_sprite->transform.texture_size.x
        && getPosY + ptr_attached_sprite->transform.texture_size.y + this->box_collider_height.y >= otherGetPosY + other.box_collider_height.x
        && getPosY + this->box_collider_height.x <= otherGetPosY + other.box_collider_height.y + other.ptr_attached_sprite->transform.texture_size.y)
    {
        if (other.is_solid && this->is_solid)
            checkPositions(other, jIndex);
        this->position_data.position[this->collision_cnt] = s2d::BoxColliderPositionData::Collision;
        this->collision_cnt++;
        return true;
    }
    return false;
}

void s2d::BoxCollider::reset()
{
    this->exist = false;
    this->is_solid = false;
    this->can_collide = false;
    this->box_collider_height = Vector2(0, 0);
    this->box_collider_width = Vector2(0, 0);
}

//Private functions

bool s2d::BoxCollider::checkIAndJPCollisions(int i, int j, s2d::SpriteRepository& repo)
{
    bool collided = false;
    if (repo.readAt(i)->collider.checkCollision(repo.readAt(j)->collider, j))
    {
        collided = true;
    }
    if (repo.readAt(j)->collider.checkCollision(repo.readAt(i)->collider, j))
    {
        collided = true;
    }
    return collided;
}

void s2d::BoxCollider::checkPositions(const BoxCollider& other, const int jIndex)
{
    short range = 10;

    // Right
    if (this->ptr_attached_sprite->getOrigininalPosition().x + this->ptr_attached_sprite->transform.texture_size.x + this->ptr_attached_sprite->collider.box_collider_width.y >= other.ptr_attached_sprite->getOrigininalPosition().x + other.box_collider_width.x
        && this->ptr_attached_sprite->getOrigininalPosition().x + this->ptr_attached_sprite->transform.texture_size.x + this->ptr_attached_sprite->collider.box_collider_width.y <= other.ptr_attached_sprite->getOrigininalPosition().x + other.box_collider_width.x + range)
    {
        this->position_data.position[this->collision_cnt] = s2d::BoxColliderPositionData::Position::Right;
        this->collision_cnt++;
        return;
    }

    // Left

    if (this->ptr_attached_sprite->getOrigininalPosition().x + this->box_collider_width.x <= other.ptr_attached_sprite->getOrigininalPosition().x + other.ptr_attached_sprite->transform.texture_size.x + other.box_collider_width.y
        && this->ptr_attached_sprite->getOrigininalPosition().x + this->box_collider_width.x + range >= other.ptr_attached_sprite->getOrigininalPosition().x + other.ptr_attached_sprite->transform.texture_size.x + other.box_collider_width.y)
    {
        this->position_data.position[this->collision_cnt] = s2d::BoxColliderPositionData::Position::Left;
        this->collision_cnt++;
        return;
    }

    // Down

    if (this->ptr_attached_sprite->getOrigininalPosition().y + this->ptr_attached_sprite->transform.texture_size.y + this->ptr_attached_sprite->collider.box_collider_height.y >= other.ptr_attached_sprite->getOrigininalPosition().y + other.box_collider_height.x
        && (this->ptr_attached_sprite->getOrigininalPosition().y + this->ptr_attached_sprite->transform.texture_size.y + this->ptr_attached_sprite->collider.box_collider_height.y <= other.ptr_attached_sprite->getOrigininalPosition().y + other.box_collider_height.x + range))
    {
        this->position_data.position[this->collision_cnt] = s2d::BoxColliderPositionData::Position::Down;
        this->collision_cnt++;
        return;
    }

    // Top

    // TODO: VALID CHECK FOR TOP!
    //if (this->sprite->getOrigininalPosition().y + this->boxColliderHeightUpOrDown.x <= other.sprite->getOrigininalPosition().y + other.sprite->transform.textureSize.y + other.boxColliderHeightUpOrDown.y
    //    && (this->sprite->getOrigininalPosition().y + this->sprite->collider.boxColliderHeightUpOrDown.y + range >= other.sprite->getOrigininalPosition().y + other.sprite->transform.textureSize.y + other.boxColliderHeightUpOrDown.y))
    //{
    this->position_data.position[this->collision_cnt] = s2d::BoxColliderPositionData::Position::Up;
    this->collision_cnt++;
    return;
    //
}

//Static functions

void s2d::BoxCollider::checkCollisions(s2d::SpriteRepository& repo)
{
    for (size_t i = 0; i < repo.amount(); i++)
    {
        for (size_t j = i + 1; j < repo.amount(); j++)
        {
            if (checkIAndJPCollisions((int)i, (int)j, repo))
            {
                s2d::Sprite* iS = repo.readAt(i);
                s2d::Sprite* jS = repo.readAt(j);

                iS->collider.colliding_sprite = jS;
                jS->collider.colliding_sprite = iS;
            }
        }
    }

    // set the colliding sprite to nulltpr if the sprite doens't collide
    for (int i = 0; i < repo.amount(); i++)
    {
        s2d::Sprite* const sprite = repo.readAt(i);
        if (sprite->collider.position_data.isEverythingUnknown())
        {
            sprite->collider.colliding_sprite = nullptr;
        }
    }
}
