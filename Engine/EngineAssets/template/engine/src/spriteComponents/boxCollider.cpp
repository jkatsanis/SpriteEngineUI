#include "boxCollider.h"

//Includint sprite.h here because we cant in the.h file because we include boxcollider.h already in the sprite.h file
#include <physicalComponents/sprite.h>

//Its always important to give everything a "default value"

s2d::BoxCollider::BoxCollider()
    : isSolid(false), canCollide(false), exists(false), sprite(nullptr), collidingSprite(nullptr), collisionCnt(0) { }

s2d::BoxCollider::BoxCollider(s2d::Sprite* sprite)
   : exists(false), collidingSprite(nullptr), sprite(sprite), canCollide(false), isSolid(false), collisionCnt(0) { }


bool s2d::BoxCollider::checkCollision(s2d::BoxCollider& other, const int jIndex)
{
    //Already got collision detection working

    if (!this->exists || !other.exists)
    {
        return false;
    }

    //May need to do better sprite pos detection!
    //Using for example this->boxColliderWidthLeftOrRight.y when we getPosX and add sprite size because we can adjust the right widtg in the inspector and the right width is y

    float getPosX = sprite->getSprite().getPosition().x;
    float getPosY = sprite->getSprite().getPosition().y;

    float otherGetPosX = other.sprite->getSprite().getPosition().x;
    float otherGetPosY = other.sprite->getSprite().getPosition().y;

    if (getPosX + sprite->transform.textureSize.x + this->boxColliderWidthLeftOrRight.y >= otherGetPosX + other.boxColliderWidthLeftOrRight.x
        && getPosX + this->boxColliderWidthLeftOrRight.x <= otherGetPosX + other.boxColliderWidthLeftOrRight.y + other.sprite->transform.textureSize.x
        && getPosY + sprite->transform.textureSize.y + this->boxColliderHeightUpOrDown.y >= otherGetPosY + other.boxColliderHeightUpOrDown.x
        && getPosY + this->boxColliderHeightUpOrDown.x <= otherGetPosY + other.boxColliderHeightUpOrDown.y + other.sprite->transform.textureSize.y)
    {
        if (other.isSolid && this->isSolid)
            checkPositions(other, jIndex);
        this->positionData.position[this->collisionCnt] = s2d::BoxColliderPositionData::Collision;
        this->collisionCnt++;
        return true;
    }
    return false;
}

//Private functions

#pragma region  Checking_positions_(setting sprite relative position for collisions (well no shit collision.cpp))

bool s2d::BoxCollider::checkIAndJPCollisions(int i, int j)
{
    bool collided = false;
    if (s2d::Sprite::activeSprites[i]->collider.checkCollision(s2d::Sprite::activeSprites[j]->collider, j))
    {
        collided = true;
    }
    if (s2d::Sprite::activeSprites[j]->collider.checkCollision(s2d::Sprite::activeSprites[i]->collider, j))
    {
        collided = true;
    }
    return collided;
}

void s2d::BoxCollider::checkPositions(const BoxCollider& other, const int jIndex)
{
    short range = 10;

    // Right
    if (this->sprite->getOrigininalPosition().x + this->sprite->transform.textureSize.x + this->sprite->collider.boxColliderWidthLeftOrRight.y >= other.sprite->getOrigininalPosition().x + other.boxColliderWidthLeftOrRight.x
        && this->sprite->getOrigininalPosition().x + this->sprite->transform.textureSize.x  + this->sprite->collider.boxColliderWidthLeftOrRight.y <= other.sprite->getOrigininalPosition().x + other.boxColliderWidthLeftOrRight.x + range)
    {
        this->positionData.position[this->collisionCnt] = s2d::BoxColliderPositionData::Position::Right;
        this->collisionCnt++;
        return;
    }

    // Left

    if (this->sprite->getOrigininalPosition().x + this->boxColliderWidthLeftOrRight.x <= other.sprite->getOrigininalPosition().x + other.sprite->transform.textureSize.x + other.boxColliderWidthLeftOrRight.y
        && this->sprite->getOrigininalPosition().x + this->boxColliderWidthLeftOrRight.x + range>= other.sprite->getOrigininalPosition().x + other.sprite->transform.textureSize.x + other.boxColliderWidthLeftOrRight.y)
    {
        this->positionData.position[this->collisionCnt] = s2d::BoxColliderPositionData::Position::Left;
        this->collisionCnt++;
        return;
    }

    // Down

    if (this->sprite->getOrigininalPosition().y + this->sprite->transform.textureSize.y + this->sprite->collider.boxColliderHeightUpOrDown.y >= other.sprite->getOrigininalPosition().y + other.boxColliderHeightUpOrDown.x
        && (this->sprite->getOrigininalPosition().y + this->sprite->transform.textureSize.y + this->sprite->collider.boxColliderHeightUpOrDown.y <= other.sprite->getOrigininalPosition().y + other.boxColliderHeightUpOrDown.x + range))
    {
        this->positionData.position[this->collisionCnt] = s2d::BoxColliderPositionData::Position::Down;
        this->collisionCnt++;
        return;
    }

    // Top

    // TODO: VALID CHECK FOR TOP!
    //if (this->sprite->getOrigininalPosition().y + this->boxColliderHeightUpOrDown.x <= other.sprite->getOrigininalPosition().y + other.sprite->transform.textureSize.y + other.boxColliderHeightUpOrDown.y
    //    && (this->sprite->getOrigininalPosition().y + this->sprite->collider.boxColliderHeightUpOrDown.y + range >= other.sprite->getOrigininalPosition().y + other.sprite->transform.textureSize.y + other.boxColliderHeightUpOrDown.y))
    //{
        std::cout << "hi";
        this->positionData.position[this->collisionCnt] = s2d::BoxColliderPositionData::Position::Up;
        this->collisionCnt++;
        return;
    //
}

#pragma endregion


//Static functions

void s2d::BoxCollider::checkCollisions()
{
    for (int i = 0; i < s2d::Sprite::activeSprites.size(); i++)
    {      
        for (int j = i + 1; j < s2d::Sprite::activeSprites.size(); j++)
        {
            if (checkIAndJPCollisions(i, j))
            {
                s2d::Sprite::activeSprites[i]->collider.collidingSprite = s2d::Sprite::activeSprites[j];
                s2d::Sprite::activeSprites[j]->collider.collidingSprite = s2d::Sprite::activeSprites[i];
            }       
        }
    }

    // set the colliding sprite to nulltpr if the sprite doens't collide
    for (int i = 0; i < s2d::Sprite::activeSprites.size(); i++)
    {
        if (s2d::Sprite::activeSprites[i]->collider.positionData.isEverythingUnknown())
        {
            s2d::Sprite::activeSprites[i]->collider.collidingSprite = nullptr;
        }
    }
}
