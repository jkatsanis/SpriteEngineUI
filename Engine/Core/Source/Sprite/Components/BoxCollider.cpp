#include "BoxCollider.h"

#include "Sprite.h"
#include "SpriteRepository.h"

// Constructor / Destructor

spe::BoxCollider::BoxCollider()
{
    this->init();
}

spe::BoxCollider::BoxCollider(spe::Sprite* sprite, spe::BoxCollider& rhs)
{
    this->init();
    this->ptr_sprite = sprite;
    this->box_collider_height = this->box_collider_height;
    this->box_collider_width = this->box_collider_width;
    this->exist = this->exist;
    this->is_solid = this->is_solid;
}

spe::BoxCollider::BoxCollider(spe::Sprite* ptr_sprite)
{
    this->init();
    this->ptr_sprite = ptr_sprite;
}

void spe::BoxCollider::init()
{
    this->base_component = false;
    this->ptr_sprite = nullptr;
    this->can_collide = false;
    this->is_solid = false;
    this->exist = false;

    this->left = false;
    this->right = false;
    this->up = false;
    this->down = false;
    this->collided = false;

    this->m_got_down = false;
    this->m_got_left = false;
    this->m_got_right = false;
    this->m_got_up = false;
}

bool spe::BoxCollider::CheckCollision(spe::BoxCollider& other)
{
    spe::Sprite* ptr_attached_sprite = this->ptr_sprite;
    if (!this->exist || !other.exist)
    {
        return false;
    }

    float getPosX = ptr_attached_sprite->transform.getOrigininalPosition().x;
    float getPosY = ptr_attached_sprite->transform.getOrigininalPosition().y;

    float otherGetPosX = other.ptr_sprite->transform.getOrigininalPosition().x;
    float otherGetPosY = other.ptr_sprite->transform.getOrigininalPosition().y;

    bool isHorizontalOverlapLeft =
        getPosX + ptr_attached_sprite->transform.texture_size.x + this->box_collider_width.y >= otherGetPosX + other.box_collider_width.x;

    bool isHorizontalOverlapRight =
        getPosX + this->box_collider_width.x <= otherGetPosX + other.box_collider_width.y + other.ptr_sprite->transform.texture_size.x;

    bool isVerticalOverlapTop =
        getPosY + ptr_attached_sprite->transform.texture_size.y + this->box_collider_height.y >= otherGetPosY + other.box_collider_height.x;

    bool isVerticalOverlapBottom =
        getPosY + this->box_collider_height.x <= otherGetPosY + other.box_collider_height.y + other.ptr_sprite->transform.texture_size.y;

    if (isHorizontalOverlapLeft && isHorizontalOverlapRight && isVerticalOverlapTop && isVerticalOverlapBottom) {
        // Your code here
        other.collided = true;
        other.collided_in_frame = true;
        this->collided = true;
        this->collided_in_frame = true;

        if (this->is_solid && other.is_solid)
        {
            this->CheckCollisionPosition(other);
        }
        return true;
    }
    return false;
}

void spe::BoxCollider::CheckCollisionPosition(spe::BoxCollider& other)
{
    const short range = 10;

    // OTHER
    const float other_right = other.ptr_sprite->transform.getOrigininalPosition().x + other.ptr_sprite->transform.texture_size.x + other.box_collider_width.y;
    const float other_left = other.ptr_sprite->transform.getOrigininalPosition().x + other.box_collider_width.x;

    const float other_bottom = other.ptr_sprite->transform.getOrigininalPosition().y + other.ptr_sprite->transform.texture_size.y + other.box_collider_height.y;
    const float other_top = other.ptr_sprite->transform.getOrigininalPosition().y + other.box_collider_height.x;

    // THIS    
    const float this_bottom = this->ptr_sprite->transform.getOrigininalPosition().y + this->ptr_sprite->transform.texture_size.y + this->box_collider_height.y;
    const float this_top = this->ptr_sprite->transform.getOrigininalPosition().y + this->box_collider_height.x;

    const float this_right = this->ptr_sprite->transform.getOrigininalPosition().x + this->ptr_sprite->transform.texture_size.x + this->ptr_sprite->collider.box_collider_width.y;
    const float this_left = this->ptr_sprite->transform.getOrigininalPosition().x + this->box_collider_width.x;

    // Right
    if (this_right >= other_left
        && this_right <= other_left + range)
    {
        other.m_got_left = true;
        other.left = true,

        this->m_got_right = true;
        this->right = true;
        return;
    }


    // Left

    if (this_left <= other_right
        && this_left + range >= other_right)
    {
        other.m_got_right = true;
        other.right = true,

        this->m_got_left = true;
        this->left = true;
        return;
    }

    // Down
    if (this_bottom >= other_top
        && this_bottom <= other_top + range)
    {
        other.m_got_up = true;
        other.up = true,

        this->m_got_down = true;
        this->down = true;
        return;
    }

    other.m_got_down = true;
    other.down = true;

    this->m_got_up = true;
    this->up = true;
    return;
}

void spe::BoxCollider::ResetPosition()
{
    this->collided_sprite_map.clear();
    this->collided = false;
    this->right = false;
    this->left = false;
    this->down = false;
    this->up = false;
}
 
// Public functions

void spe::BoxCollider::reset() 
{
    this->exist = false;
    this->is_solid = false;
    this->can_collide = false;
    this->box_collider_height = Vector2(0, 0);
    this->box_collider_width = Vector2(0, 0);
}

void spe::BoxCollider::Update(spe::SpriteRepository& tocheck)
{
    spe::Sprite* sprite = this->ptr_sprite;

    sprite->collider.collided_in_frame = false;
    sprite->collider.m_got_down = false;
    sprite->collider.m_got_up = false;
    sprite->collider.m_got_right = false;
    sprite->collider.m_got_left = false;


    spe::Sprite* i_s = sprite;
    
    std::list<spe::Sprite*>& sprites = tocheck.GetSprites();


    for (auto it = sprites.begin(); it != sprites.end(); ++it)
    {
        spe::Sprite* j_s = *it;

        if (i_s->getId() == j_s->getId())
        {
            continue;
        }

        if (this->CheckCollision(j_s->collider))
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


    if (!sprite->collider.collided_in_frame)
    {
        sprite->collider.ResetPosition();
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

