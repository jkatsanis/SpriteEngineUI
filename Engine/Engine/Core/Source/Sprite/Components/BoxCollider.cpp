#include "BoxCollider.h"
#include "Sprite/Sprite.h"
#include "Sprite/SpriteRepository.h"
#include "Core/Time.h"
#include "Core/EngineData.h" // Include EngineData
#include <algorithm>
#include <cmath>
#include <limits>

// Constructor / Destructor

spe::BoxCollider::BoxCollider()
{
    this->Init();
}

spe::BoxCollider::BoxCollider(spe::Sprite* sprite, spe::BoxCollider& rhs)
{
    this->Init();
    this->ptr_Sprite = sprite;
    this->Height = rhs.Height;
    this->Width = rhs.Width;
    this->Exist = rhs.Exist;
    this->IsSolid = rhs.IsSolid;
    this->Controller = rhs.Controller;
}

spe::BoxCollider::BoxCollider(spe::Sprite* ptr_sprite)
{
    this->Init();
    this->ptr_Sprite = ptr_sprite;
}

void spe::BoxCollider::Init()
{
    this->BaseComponent = false;
    this->ptr_Sprite = nullptr;
    this->CanCollide = false;
    this->IsSolid = false;
    this->Exist = false;

    this->Left = false;
    this->Right = false;
    this->Up = false;
    this->Down = false;
    this->Collided = false;

    this->m_GotDown = false;
    this->m_GotLeft = false;
    this->m_GotRight = false;
    this->m_GotUp = false;

    this->Controller = false;
}

bool spe::BoxCollider::CheckCollision(spe::BoxCollider& other)
{
    spe::Sprite* ptr_attached_sprite = this->ptr_Sprite;
    if (!this->Exist || !other.Exist)
    {
        return false;
    }

    // World Space Coordinates (Center)
    const spe::Vector2 thisPos = ptr_attached_sprite->Transform.GetPosition();
    const spe::Vector2 otherPos = other.ptr_Sprite->Transform.GetPosition();

    // Half Sizes
    const float thisHalfW = ptr_attached_sprite->Transform.TextureSize.X / 2.0f;
    const float thisHalfH = ptr_attached_sprite->Transform.TextureSize.Y / 2.0f;
    const float otherHalfW = other.ptr_Sprite->Transform.TextureSize.X / 2.0f;
    const float otherHalfH = other.ptr_Sprite->Transform.TextureSize.Y / 2.0f;

    // Calculate Edges (World Space)
    // Width.X = Left Offset, Width.Y = Right Offset
    // Height.X = Top Offset (shrinks from top), Height.Y = Bottom Offset (shrinks from bottom)

    const float thisLeft = (thisPos.X - thisHalfW) + this->Width.X;
    const float thisRight = (thisPos.X + thisHalfW) + this->Width.Y;
    const float thisTop = (thisPos.Y + thisHalfH) - this->Height.X;
    const float thisBottom = (thisPos.Y - thisHalfH) - this->Height.Y;

    const float otherLeft = (otherPos.X - otherHalfW) + other.Width.X;
    const float otherRight = (otherPos.X + otherHalfW) + other.Width.Y;
    const float otherTop = (otherPos.Y + otherHalfH) - other.Height.X;
    const float otherBottom = (otherPos.Y - otherHalfH) - other.Height.Y;

    bool isHorizontalOverlap = (thisRight >= otherLeft) && (thisLeft <= otherRight);
    bool isVerticalOverlap = (thisTop >= otherBottom) && (thisBottom <= otherTop);

    if (isHorizontalOverlap && isVerticalOverlap)
    {
        other.Collided = true;
        other.CollidedInFrame = true;
        this->Collided = true;
        this->CollidedInFrame = true;

        if (this->IsSolid && other.IsSolid)
        {
            this->CheckCollisionPosition(other);
        }
        return true;
    }
    return false;
}

void spe::BoxCollider::CheckCollisionPosition(spe::BoxCollider& other) noexcept
{
    const spe::Vector2 thisPos = this->ptr_Sprite->Transform.GetPosition();
    const spe::Vector2 otherPos = other.ptr_Sprite->Transform.GetPosition();

    const float thisHalfW = this->ptr_Sprite->Transform.TextureSize.X / 2.0f;
    const float thisHalfH = this->ptr_Sprite->Transform.TextureSize.Y / 2.0f;
    const float otherHalfW = other.ptr_Sprite->Transform.TextureSize.X / 2.0f;
    const float otherHalfH = other.ptr_Sprite->Transform.TextureSize.Y / 2.0f;

    const float thisLeft = (thisPos.X - thisHalfW) + this->Width.X;
    const float thisRight = (thisPos.X + thisHalfW) + this->Width.Y;
    const float thisTop = (thisPos.Y + thisHalfH) - this->Height.X;
    const float thisBottom = (thisPos.Y - thisHalfH) - this->Height.Y;

    const float otherLeft = (otherPos.X - otherHalfW) + other.Width.X;
    const float otherRight = (otherPos.X + otherHalfW) + other.Width.Y;
    const float otherTop = (otherPos.Y + otherHalfH) - other.Height.X;
    const float otherBottom = (otherPos.Y - otherHalfH) - other.Height.Y;

    // Calculate overlaps

    // Right: This hits Other's Left
    float overlapRight = thisRight - otherLeft;

    // Left: This hits Other's Right
    float overlapLeft = otherRight - thisLeft;

    // Down: This hits Other's Top (Falling down)
    float overlapDown = otherTop - thisBottom;

    // Up: This hits Other's Bottom (Jumping up)
    float overlapUp = thisTop - otherBottom;

    // Find minimum overlap
    float minOverlap = std::numeric_limits<float>::max();
    int axis = -1; // 0: Right, 1: Left, 2: Down, 3: Up

    if (overlapRight < minOverlap) { minOverlap = overlapRight; axis = 0; }
    if (overlapLeft < minOverlap) { minOverlap = overlapLeft; axis = 1; }
    if (overlapDown < minOverlap) { minOverlap = overlapDown; axis = 2; }
    if (overlapUp < minOverlap) { minOverlap = overlapUp; axis = 3; }

    // Apply collision based on minimum overlap
    if (axis == 0) // Right
    {
        other.m_GotLeft = true;
        other.Left = true;

        this->m_GotRight = true;
        this->Right = true;
    }
    else if (axis == 1) // Left
    {
        other.m_GotRight = true;
        other.Right = true;

        this->m_GotLeft = true;
        this->Left = true;
    }
    else if (axis == 2) // Down
    {
        other.m_GotUp = true;
        other.Up = true;

        this->m_GotDown = true;
        this->Down = true;
    }
    else if (axis == 3) // Up
    {
        other.m_GotDown = true;
        other.Down = true;

        this->m_GotUp = true;
        this->Up = true;
    }
}

void spe::BoxCollider::ResetPosition() noexcept
{ 
    this->CollidedSprites.clear();
    this->Collided = false;
    this->Right = false;
    this->Left = false;
    this->Down = false;
    this->Up = false;
}
 
// Public functions

void spe::BoxCollider::Reset() 
{
    this->Exist = false;
    this->IsSolid = false;
    this->CanCollide = false;
    this->Height = Vector2(0, 0);
    this->Width = Vector2(0, 0);
    this->Collided = false;
    this->Left = false;
    this->Right = false;
    this->Up = false;
    this->Down = false;
    this->CollidedInFrame = false;
    this->CollidedSprites.clear();
}

void spe::BoxCollider::Update(spe::SpriteRepository& tocheck)
{
    if (!this->Controller)
    {
        return;
    }

    spe::Sprite* sprite = this->ptr_Sprite;

    sprite->Collider.CollidedInFrame = false;
    sprite->Collider.m_GotDown = false;
    sprite->Collider.m_GotUp = false;
    sprite->Collider.m_GotRight = false;
    sprite->Collider.m_GotLeft = false;


    spe::Sprite* i_s = sprite;
    
    std::list<spe::Sprite*>& sprites = tocheck.GetSprites();


    for (auto it = sprites.begin(); it != sprites.end(); ++it)
    {
        spe::Sprite* j_s = *it;

        if (i_s->GetId() == j_s->GetId())
        {
            continue;
        }

        if (this->CheckCollision(j_s->Collider))
        {
            i_s->Collider.CollidedSprites[j_s->GetId()] = j_s;
            j_s->Collider.CollidedSprites[i_s->GetId()] = i_s;
        }
        else
        {
            i_s->Collider.CollidedSprites.erase(j_s->GetId());
            j_s->Collider.CollidedSprites.erase(i_s->GetId());
        }
    }


    if (!sprite->Collider.CollidedInFrame)
    {
        sprite->Collider.ResetPosition();
    }
    if (!sprite->Collider.m_GotDown)
    {
        sprite->Collider.Down = false;
    }
    if (!sprite->Collider.m_GotUp)
    {
        sprite->Collider.Up = false;
    }
    if (!sprite->Collider.m_GotRight)
    {
        sprite->Collider.Right = false;
    }
    if (!sprite->Collider.m_GotLeft)
    {
        sprite->Collider.Left = false;
    }
}

// User utility

spe::Sprite* spe::BoxCollider::CollidedWithTag(const std::string& tag)
{
    for (const auto& pair : this->CollidedSprites)
    {
        if (pair.second->Tag == tag)
        {
            return pair.second;
        }
    }

    return nullptr;
}

spe::Sprite* spe::BoxCollider::CollidedWithName(const std::string& name)
{
    for (const auto& pair : this->CollidedSprites)
    {
        if (pair.second->Name == name)
        {
            return pair.second;
        }
    }

    return nullptr;
}

bool spe::BoxCollider::ProcessSprite(spe::Sprite* other, const spe::Camera& camera)
{
    if (other->Name == "CameraCollider")
    {
        return false;
    }

    float zoomfactor = camera.GetZoom();
    if (other->Light.Exist)
    {
        zoomfactor *= 2;
    }
    if (camera.GetZoom() < 0.5f)
    {
		zoomfactor *= 8;
	}

    const spe::Vector2 pos = spe::Vector2(camera.Position.X, camera.Position.Y * -1);
    spe::BoxCollider::s_ptr_CameraCollider->Transform.SetPosition(pos);

    // Use dynamic window size for culling box
    float halfWidth = spe::EngineData::s_WindowWidth / 2.0f;
    float halfHeight = spe::EngineData::s_WindowHeight / 2.0f;

    spe::BoxCollider::s_ptr_CameraCollider->Collider.Width = Vector2(-halfWidth * zoomfactor, halfWidth * zoomfactor);
    spe::BoxCollider::s_ptr_CameraCollider->Collider.Height = Vector2(-halfHeight * zoomfactor, halfHeight * zoomfactor);

    const spe::Sprite* this_s = spe::BoxCollider::s_ptr_CameraCollider;

    // Updated to use World Coordinates
    const spe::Vector2 thisPos = this_s->Transform.GetPosition();
    const spe::Vector2 otherPos = other->Transform.GetPosition();

    const float thisHalfW = this_s->Transform.TextureSize.X / 2.0f;
    const float thisHalfH = this_s->Transform.TextureSize.Y / 2.0f;

    const float otherHalfW = other->Transform.TextureSize.X / 2.0f;
    const float otherHalfH = other->Transform.TextureSize.Y / 2.0f;

    // Camera Collider Edges
    const float thisLeft = (thisPos.X - thisHalfW) + this_s->Collider.Width.X;
    const float thisRight = (thisPos.X + thisHalfW) + this_s->Collider.Width.Y;
    const float thisTop = (thisPos.Y + thisHalfH) - this_s->Collider.Height.X;
    const float thisBottom = (thisPos.Y - thisHalfH) - this_s->Collider.Height.Y;

    // Other Sprite Edges
    const float otherLeft = (otherPos.X - otherHalfW) + other->Collider.Width.X;
    const float otherRight = (otherPos.X + otherHalfW) + other->Collider.Width.Y;
    const float otherTop = (otherPos.Y + otherHalfH) - other->Collider.Height.X;
    const float otherBottom = (otherPos.Y - otherHalfH) - other->Collider.Height.Y;

    bool isHorizontalOverlap = (thisRight >= otherLeft) && (thisLeft <= otherRight);
    bool isVerticalOverlap = (thisTop >= otherBottom) && (thisBottom <= otherTop);

    return (isHorizontalOverlap && isVerticalOverlap);
}

void spe::BoxCollider::InitCameraCollider(spe::LightRepository& repo)
{
    std::string path = PATH_TO_RESSOURCES + PATH_SYMBOL  + "Sprites" + PATH_SYMBOL + "CamColl.png";
    spe::BoxCollider::s_ptr_CameraCollider = new spe::Sprite(
        "CameraCollider",
        spe::Vector2(0, 0),
        path,
        repo
    );

    s_ptr_CameraCollider->Collider.Width = Vector2(-960, 960);
    s_ptr_CameraCollider->Collider.Height = Vector2(-540, 540);

}

void spe::BoxCollider::DeleteCameraCollider()
{
    delete spe::BoxCollider::s_ptr_CameraCollider;
}

spe::Sprite* spe::BoxCollider::s_ptr_CameraCollider = nullptr;