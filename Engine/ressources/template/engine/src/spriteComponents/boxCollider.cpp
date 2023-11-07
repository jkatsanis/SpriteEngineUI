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
    this->collided_sprite_map.clear();
    this->collided = false;
    this->right = false;
    this->left = false;
    this->down = false;
    this->up = false;
}

s2d::Sprite* s2d::BoxCollider::collidedWithId(uint32_t id)
{
    auto it = this->collided_sprite_map.find(id);
    return it != this->collided_sprite_map.end()
        ? it->second
        : nullptr;
}

s2d::Sprite* s2d::BoxCollider::collidedWithTag(const std::string& tag)
{
    for (const auto& [key, obj] : this->collided_sprite_map) 
    {
        if (obj->tag == tag)
        {
            return obj;
        }
    }
    return nullptr;
}

s2d::Sprite* s2d::BoxCollider::collidedWithName(const std::string& name)
{
    for (const auto& [key, obj] : this->collided_sprite_map)
    {
        if (obj->name == name)
        {
            return obj;
        }
    }
    return nullptr;
}

//Private functions

void s2d::BoxCollider::checkPositions(const BoxCollider& other)
{
    const short range = 10;

    // OTHER
    const float other_right = other.ptr_attached_sprite->getOrigininalPosition().x + other.ptr_attached_sprite->transform.texture_size.x + other.box_collider_width.y;
    const float other_left = other.ptr_attached_sprite->getOrigininalPosition().x + other.box_collider_width.x;

    const float other_bottom = other.ptr_attached_sprite->getOrigininalPosition().y + other.ptr_attached_sprite->transform.texture_size.y + other.box_collider_height.y;
    const float other_top = other.ptr_attached_sprite->getOrigininalPosition().y + other.box_collider_height.x;

    // THIS    
    const float this_bottom = this->ptr_attached_sprite->getOrigininalPosition().y + this->ptr_attached_sprite->transform.texture_size.y + this->box_collider_height.y;
    const float this_top = this->ptr_attached_sprite->getOrigininalPosition().y + this->box_collider_height.x;

    const float this_right = this->ptr_attached_sprite->getOrigininalPosition().x + this->ptr_attached_sprite->transform.texture_size.x + this->ptr_attached_sprite->collider.box_collider_width.y;
    const float this_left = this->ptr_attached_sprite->getOrigininalPosition().x + this->box_collider_width.x;

    // Right
    if (this_right >= other_left
        && this_right <= other_left + range)
    {   
        this->m_got_right = true;
        this->right = true;
        return;
    }


    // Left

    if (this_left <= other_right
        && this_left + range >= other_right)
    {
        this->m_got_left = true;
        this->left = true;
        return;
    }

    // Down
    if (this_bottom >= other_top
        && this_bottom <= other_top + range)
    {
        this->m_got_down = true;
        this->down = true;
        return;
    }

    this->m_got_up = true;
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
        ds->collider.m_got_down = false;
        ds->collider.m_got_up = false;
        ds->collider.m_got_right = false;
        ds->collider.m_got_left = false;
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
                i_s->collider.collided_sprite_map[j_s->getId()] = j_s;
                j_s->collider.collided_sprite_map[i_s->getId()] = i_s;
            }
            else
            {
                i_s->collider.collided_sprite_map.erase(j_s->getId());
                j_s->collider.collided_sprite_map.erase(i_s->getId());
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
        if (!sprite->collider.m_got_down)
        {
            sprite->collider.down = false;
        }
        if (!sprite->collider.m_got_up)
        {
            sprite->collider.up = false;
        }
        if (!sprite->collider.m_got_right)
        {
            sprite->collider.right = false;
        }
        if (!sprite->collider.m_got_left)
        {
            sprite->collider.left = false;
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
        other.collided = true;
        other.collided_in_frame = true;
        rhs.collided = true;
        rhs.collided_in_frame = true;

        if (rhs.is_solid && other.is_solid)
        {
            rhs.checkPositions(other);
            other.checkPositions(rhs);
        }
        return true;
    }
    return false;
}