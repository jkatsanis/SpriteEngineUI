#include "Sprite.h"

#include "Camera/Camera.h"

//Constructor

spe::Sprite::Sprite()
{
	this->m_ID = 0;
	this->m_ParentID = 0;
	this->ptr_Parent = nullptr;
	this->m_SetId = false;
	this->Name = "Unknown";
	this->Transform.SetPosition(spe::Vector2(0, 0));
	this->m_Texture = nullptr;
}

spe::Sprite::Sprite(std::string name, spe::Vector2 spawnPosition, std::string path, spe::LightRepository& lightrep)
	: Name(name)
{
	this->InitVariables(spawnPosition, path, lightrep);
}

spe::Sprite::Sprite(spe::Sprite& rhs)
{
	this->InitVariables(rhs.Transform.GetPosition(), rhs.SpriteRenderer.Path, *rhs.Light.m_ptr_LighRepository);

	this->Collider = spe::BoxCollider(this, rhs.Collider);
	this->Transform = spe::Transform(this, rhs.Transform);
	this->Animator = spe::Animator(this, rhs.Animator);
	this->Physicsbody = spe::PhsysicsBody(this, rhs.Physicsbody);
	this->SpriteRenderer = spe::SpriteRenderer(rhs.SpriteRenderer);
	this->Light = spe::Light(this, rhs.Light);
	this->Prefab = spe::Prefab(this, rhs.Prefab);
	this->Name = rhs.Name;

	this->Tag = rhs.Tag;

	// Initing the childs
	for (size_t i = 0; i < rhs.ptr_Childs.size(); i++)
	{
		spe::Sprite* copy_child = new spe::Sprite(*rhs.ptr_Childs[i]);
		copy_child->ptr_Parent = this;
		this->ptr_Childs.push_back(copy_child);
	}
}

spe::Sprite::~Sprite()
{
	this->ClearAllChilds();
	this->ClearParentData();

	this->Light.DeleteLight();

	delete this->m_Texture;
	this->m_Texture = nullptr;
}

//Public functions

//////////////////////////////////////
//// USER FUNCTIONS 
/////////////////////////////////////

void spe::Sprite::SetSpriteTexture(const std::string& path)
{
	if (!this->m_Texture->loadFromFile(path))
	{
		const std::string error = "File " + path + " was not found!, Sprite.cpp 59";
		spe::Log::LogString(error);
	}
	this->SetSpriteTexture(*this->m_Texture, path);
}

void spe::Sprite::SetSpriteTexture(const std::string& path, const spe::Vector2& scale)
{
	if (!this->m_Texture->loadFromFile(path))
	{
		const std::string error = "File " + path + " was not found!, Sprite.cpp 68";
		spe::Log::LogString(error);
	}
	this->SetSpriteTexture(*this->m_Texture, path);
	this->Transform.SetScale(scale, true);
}

void spe::Sprite::SetSpriteTexture(const sf::Texture& texture, const std::string& path)
{
	this->m_Sprite.setTexture(texture, true);
	this->Transform.SetScale(this->Transform.GetScale(), true);
	this->SpriteRenderer.Path = path;

	this->Transform.SetOrigin();
}

void spe::Sprite::SetId(const int32_t id) noexcept
{
	this->m_ID = id;
}

void spe::Sprite::ClearParentData()
{
	if (this->ptr_Parent != nullptr)
	{
		this->ptr_Parent->RemoveChild(this);
		this->ptr_Parent = nullptr;
	}
	this->m_ParentID = 0;
}

void spe::Sprite::SetParent(spe::Sprite* spriteParent)
{
	if (spriteParent == nullptr)
	{
		return;
	}

	// Clean up (Before) parent
	if (this->ptr_Parent != nullptr)
	{
		this->ptr_Parent->RemoveChild(this);
		this->ptr_Parent = nullptr;
	}
	this->m_ParentID = spriteParent->GetId();
	this->ptr_Parent = spriteParent;

	spe::Sprite* child = this;
	spe::Vector2 distance = spe::Vector2(spriteParent->Transform.GetPosition() - child->Transform.GetPosition());
	child->Transform.PositionToParent = distance;

	spriteParent->ptr_Childs.push_back(this);
}

void spe::Sprite::RemoveChild(const spe::Sprite* child)
{
	if (child == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < this->ptr_Childs.size(); i++)
	{
		if (child->GetId() == this->ptr_Childs[i]->GetId())
		{
			this->ptr_Childs.erase(this->ptr_Childs.begin() + i);
			return;
		}
	}
}

//Private functions

void spe::Sprite::InitVariables(spe::Vector2 spawnPos, std::string path, spe::LightRepository& lightrep)
{
	// Components
	this->Transform = spe::Transform(this);
	this->Animator = spe::Animator(this);
	this->Collider = spe::BoxCollider(this);
	this->Physicsbody = spe::PhsysicsBody(this);
	this->Prefab = spe::Prefab(this);
	this->Light = spe::Light(this, &lightrep);

	// ID's get set in the sprite repo!!
	this->Tag = "none";
	this->m_Texture = new sf::Texture();
	this->m_ParentID = -1;
	this->m_ID = -1;
	this->ptr_Parent = nullptr;
	this->ptr_Childs = std::vector<spe::Sprite*>(0);
	this->Name = Name;
	this->SpriteRenderer.Path = path;
	this->m_SetId = false;

	this->SpriteRenderer.SortinLayerIdx = 0;

	this->Transform.SetOrigin();
	this->Transform.SetScale(spe::Vector2(1, 1), true);
	this->Transform.SetRotation(0);
	this->Transform.SetPosition(spawnPos);

	this->SetSpriteTexture(path);

	this->GetSprite().setPosition(sf::Vector2f(spawnPos.X + 960, 540 - spawnPos.Y));

}

//Static functions

spe::Sprite* spe::Sprite::GetNode()
{
	if (this->ptr_Parent == nullptr)
	{
		return this;
	}
	return ptr_Parent->GetNode();
}

bool spe::Sprite::UseSprite(const spe::Camera& camera, float zoom_factor)
{
	const spe::Vector2& cam_pos = spe::Vector2(camera.Position.X - 960, -camera.Position.Y + 540);

    const spe::Vector2 bounds = spe::Vector2(1920 * (camera.GetZoom() + 2), 1080 * (camera.GetZoom() + 2));

	float getPosX = cam_pos.X;
	float getPosY = cam_pos.Y;

	float otherGetPosX = this->Transform.GetOrigininalPosition().X - fabs(this->Collider.Width.X);
	float otherGetPosY = this->Transform.GetOrigininalPosition().Y - fabs(this->Collider.Height.X);

	float text_size_x = fabs(this->Collider.Width.Y) + this->Transform.TextureSize.X;
	float text_size_y = fabs(this->Collider.Height.Y) + this->Transform.TextureSize.Y;

	bool isHorizontalOverlapLeft =
		getPosX + bounds.X >= otherGetPosX;

	bool isHorizontalOverlapRight =
		getPosX <= otherGetPosX + text_size_x;

	bool isVerticalOverlapTop =
		getPosY + bounds.Y >= otherGetPosY;

	bool isVerticalOverlapBottom =
		getPosY <= otherGetPosY + text_size_y;

	return (isHorizontalOverlapLeft && isHorizontalOverlapRight && isVerticalOverlapTop && isVerticalOverlapBottom);
}

bool spe::Sprite::ContainsChild(const spe::Sprite* child) const
{
	if (child == nullptr)
	{
		return false;
	}
	bool contains = false;

	for (int i = 0; i < this->ptr_Childs.size(); i++)
	{
		const spe::Sprite* spr = this->ptr_Childs[i];
		if (child->m_ID == spr->m_ID)
		{
			return true;
		}
		if (!contains)
		{
			contains = spr->ContainsChild(child);
		}
	}
	return contains;
}

bool spe::Sprite::ContainsChild(const ImGuiTextFilter& namefilter) const
{
	bool contains = false;

	for (int i = 0; i < this->ptr_Childs.size(); i++)
	{
		const spe::Sprite* spr = this->ptr_Childs[i];
		if (namefilter.PassFilter(spr->Name.c_str()))
		{
			return true;
		}
		contains = spr->ContainsChild(namefilter);
	}
	return contains;
}
