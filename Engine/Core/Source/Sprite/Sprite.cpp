#include "Sprite.h"

//Constructor

spe::Sprite::Sprite()
{
	this->m_SetId = false;
	this->name = "Unknown";
	this->transform.SetPosition(spe::Vector2(0, 0));
	this->m_texture = nullptr;
}

spe::Sprite::Sprite(std::string name, spe::Vector2 spawnPosition, std::string path, spe::LightRepository& lightrep)
	: name(name)
{
	this->initVariables(name, spawnPosition, path, lightrep);
}

spe::Sprite::Sprite(spe::Sprite& rhs)
{
	this->initVariables(rhs.name, rhs.transform.GetPosition(), rhs.sprite_renderer.path, *rhs.light.m_ptr_LighRepository);

	this->collider = spe::BoxCollider(this, rhs.collider);
	this->transform = spe::Transform(this, rhs.transform);
	this->animator = spe::Animator(this, rhs.animator);
	this->physicsBody = spe::PhsysicsBody(rhs.physicsBody);
	this->sprite_renderer = spe::SpriteRenderer(rhs.sprite_renderer);
	this->light = spe::Light(this, rhs.light);

	this->tag = rhs.tag;

	// Initing the childs
	for (size_t i = 0; i < rhs.ptr_childs.size(); i++)
	{
		spe::Sprite* copy_child = new spe::Sprite(*rhs.ptr_childs[i]);
		copy_child->parent = this;
		this->ptr_childs.push_back(copy_child);
	}
}

spe::Sprite::~Sprite()
{
	this->clearAllChilds();
	this->clearParentData();

	this->light.deleteLight();

	delete this->m_texture;
	this->m_texture = nullptr;
}

//Public functions

//////////////////////////////////////
//// USER FUNCTIONS 
/////////////////////////////////////

void spe::Sprite::setSpriteTexture(const std::string& path)
{
	if (!this->m_texture->loadFromFile(path))
	{
		const std::string error = "File " + path + " was not found!, Sprite.cpp 59";
		spe::Log::LogString(error);
	}
	this->setSpriteTexture(*this->m_texture, path);
}

void spe::Sprite::setSpriteTexture(const std::string& path, const spe::Vector2& scale)
{
	if (!this->m_texture->loadFromFile(path))
	{
		const std::string error = "File " + path + " was not found!, Sprite.cpp 68";
		spe::Log::LogString(error);
	}
	this->setSpriteTexture(*this->m_texture, path);
	this->transform.setScale(scale, true);
}

void spe::Sprite::setSpriteTexture(const sf::Texture& texture, const std::string& path)
{
	this->m_sprite.setTexture(texture, true);
	this->transform.setScale(this->transform.getScale(), true);
	this->sprite_renderer.path = path;

	this->transform.setOrigin();
}

void spe::Sprite::setId(const int32_t id)
{
	this->m_id = id;
}

void spe::Sprite::clearParentData()
{
	if (this->parent != nullptr)
	{
		this->parent->removeChild(this);
		this->parent = nullptr;
	}
	this->m_parent_id = 0;
}

void spe::Sprite::setParent(spe::Sprite* parent)
{
	if (parent == nullptr)
	{
		return;
	}

	// Clean up (Before) parent
	if (this->parent != nullptr)
	{
		this->parent->removeChild(this);
		this->parent = nullptr;
	}
	this->m_parent_id = parent->getId();
	this->parent = parent;

	spe::Sprite* child = this;
	spe::Vector2 distance = spe::Vector2(parent->transform.GetPosition() - child->transform.GetPosition());
	child->transform.position_to_parent = distance;

	parent->ptr_childs.push_back(this);
}

void spe::Sprite::removeChild(const spe::Sprite* child)
{
	if (child == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < this->ptr_childs.size(); i++)
	{
		if (child->getId() == this->ptr_childs[i]->getId())
		{
			this->ptr_childs.erase(this->ptr_childs.begin() + i);
			return;
		}
	}
}

//Private functions

void spe::Sprite::initVariables(std::string name, spe::Vector2 spawnPos, std::string path, spe::LightRepository& lightrep)
{
	this->transform = spe::Transform(this);
	this->animator = spe::Animator(this);
	this->collider = spe::BoxCollider(this);
	this->physicsBody = spe::PhsysicsBody();
	this->prefab = spe::Prefab(this);
	this->light = spe::Light(this, &lightrep);

	// ID's get set in the sprite repo!!
	this->tag = "none";
	this->m_texture = new sf::Texture();
	this->m_parent_id = -1;
	this->m_id = -1;
	this->parent = nullptr;
	this->ptr_childs = std::vector<spe::Sprite*>(0);
	this->name = name;
	this->sprite_renderer.path = path;
	this->m_SetId = false;

	this->sprite_renderer.sorting_layer_index = 0;

	this->transform.setOrigin();
	this->transform.setScale(spe::Vector2(1, 1), true);
	this->transform.setRotation(0);
	this->transform.SetPosition(spawnPos);

	this->setSpriteTexture(path);
}

//Static functions

spe::Sprite* spe::Sprite::getNode()
{
	if (this->parent == nullptr)
	{
		return this;
	}
	return parent->getNode();
}

bool spe::Sprite::ContainsChild(const spe::Sprite* child) const
{
	if (child == nullptr)
	{
		return false;
	}
	bool contains = false;

	for (int i = 0; i < this->ptr_childs.size(); i++)
	{
		const spe::Sprite* spr = this->ptr_childs[i];
		if (child->m_id == spr->m_id)
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

bool spe::Sprite::ContainsChild(const ImGuiTextFilter& name) const
{
	bool contains = false;

	for (int i = 0; i < this->ptr_childs.size(); i++)
	{
		const spe::Sprite* spr = this->ptr_childs[i];
		if (name.PassFilter(spr->name.c_str()))
		{
			return true;
		}
		contains = spr->ContainsChild(name);
	}
	return contains;
}
