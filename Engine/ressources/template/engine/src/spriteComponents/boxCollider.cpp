#include "boxCollider.h"

//Includint sprite.h here because we cant in the.h file because we include boxcollider.h already in the sprite.h file
#include <physicalComponents/sprite.h>

//Its always important to give everything a "default value"

s2d::BoxCollider::BoxCollider()
    : is_solid(false), can_collide(false), exist(false), sprite(nullptr), collidingSprite(nullptr), collision_cnt(0) { }

s2d::BoxCollider::BoxCollider(s2d::Sprite* sprite)
    : exist(false), collidingSprite(nullptr), sprite(sprite), can_collide(false), is_solid(false), collision_cnt(0) { }


bool s2d::BoxCollider::checkCollision(s2d::BoxCollider& other, const int jIndex)
{
    //Already got collision detection working

    if (!this->exist || !other.exist)
    {
        return false;
    }

    //May need to do better sprite pos detection!
    //Using for example this->boxColliderWidthLeftOrRight.y when we getPosX and add sprite size because we can adjust the right widtg in the inspector and the right width is y

    float getPosX = sprite->getOrigininalPosition().x;
    float getPosY = sprite->getOrigininalPosition().y;

    float otherGetPosX = other.sprite->getOrigininalPosition().x;
    float otherGetPosY = other.sprite->getOrigininalPosition().y;

    if (getPosX + sprite->transform.texture_size.x + this->box_collider_width.y >= otherGetPosX + other.box_collider_width.x
        && getPosX + this->box_collider_width.x <= otherGetPosX + other.box_collider_width.y + other.sprite->transform.texture_size.x
        && getPosY + sprite->transform.texture_size.y + this->box_collider_height.y >= otherGetPosY + other.box_collider_height.x
        && getPosY + this->box_collider_height.x <= otherGetPosY + other.box_collider_height.y + other.sprite->transform.texture_size.y)
    {
        if (other.is_solid && this->is_solid)
            checkPositions(other, jIndex);
        this->position_data.position[this->collision_cnt] = s2d::BoxColliderPositionData::Collision;
        this->collision_cnt++;
        return true;
    }
    return false;
}

//Private functions

#pragma region  Checking_positions_(setting sprite relative position for collisions (well no shit collision.cpp))

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
    if (this->sprite->getOrigininalPosition().x + this->sprite->transform.texture_size.x + this->sprite->collider.box_collider_width.y >= other.sprite->getOrigininalPosition().x + other.box_collider_width.x
        && this->sprite->getOrigininalPosition().x + this->sprite->transform.texture_size.x + this->sprite->collider.box_collider_width.y <= other.sprite->getOrigininalPosition().x + other.box_collider_width.x + range)
    {
        this->position_data.position[this->collision_cnt] = s2d::BoxColliderPositionData::Position::Right;
        this->collision_cnt++;
        return;
    }

    // Left

    if (this->sprite->getOrigininalPosition().x + this->box_collider_width.x <= other.sprite->getOrigininalPosition().x + other.sprite->transform.texture_size.x + other.box_collider_width.y
        && this->sprite->getOrigininalPosition().x + this->box_collider_width.x + range >= other.sprite->getOrigininalPosition().x + other.sprite->transform.texture_size.x + other.box_collider_width.y)
    {
        this->position_data.position[this->collision_cnt] = s2d::BoxColliderPositionData::Position::Left;
        this->collision_cnt++;
        return;
    }

    // Down

    if (this->sprite->getOrigininalPosition().y + this->sprite->transform.texture_size.y + this->sprite->collider.box_collider_height.y >= other.sprite->getOrigininalPosition().y + other.box_collider_height.x
        && (this->sprite->getOrigininalPosition().y + this->sprite->transform.texture_size.y + this->sprite->collider.box_collider_height.y <= other.sprite->getOrigininalPosition().y + other.box_collider_height.x + range))
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

#pragma endregion


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

                iS->collider.collidingSprite = jS;
                jS->collider.collidingSprite = iS;
            }
        }
    }

    // set the colliding sprite to nulltpr if the sprite doens't collide
    for (int i = 0; i < repo.amount(); i++)
    {
        s2d::Sprite* const sprite = repo.readAt(i);
        if (sprite->collider.position_data.isEverythingUnknown())
        {
            sprite->collider.collidingSprite = nullptr;
        }
    }
}
