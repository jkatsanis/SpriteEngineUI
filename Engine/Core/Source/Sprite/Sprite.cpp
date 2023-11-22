#include "Sprite.h"

//Constructor

spe::Sprite::Sprite()
{
	this->name = "Unknown";
	this->transform.SetPosition(spe::Vector2(0, 0));
	this->m_texture = new sf::Texture();
}

spe::Sprite::Sprite(std::string name, spe::Vector2 spawnPosition, std::string path)
	: name(name)
{
	this->initVariables(name, spawnPosition, path);
}

spe::Sprite::Sprite(spe::Sprite& rhs)
{
	this->initVariables(rhs.name, rhs.transform.getPosition(), rhs.sprite_renderer.path);

	this->collider = spe::BoxCollider(this, rhs.collider);
	this->transform = spe::Transform(this, rhs.transform);
	this->animator = spe::Animator(this, rhs.animator);
	this->physicsBody = spe::PhsysicsBody(rhs.physicsBody);

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
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->setSpriteTexture(*this->m_texture, path);
}

void spe::Sprite::setSpriteTexture(const std::string& path, const spe::Vector2& scale)
{
	if (!this->m_texture->loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
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

spe::Vector2 spe::Sprite::getOrigininalPosition()
{
	float x = this->m_sprite.getPosition().x - this->transform.texture_size.x / 2;
	float y = this->m_sprite.getPosition().y - this->transform.texture_size.y / 2;

	return spe::Vector2(x, y);
}

void spe::Sprite::postInit()
{
	this->transform.setOrigin();
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
	spe::Vector2 distance = spe::Vector2(parent->transform.getPosition() - child->transform.getPosition());
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

void spe::Sprite::initVariables(std::string name, spe::Vector2 spawnPos, std::string path)
{
	// ID's get set in the sprite repo!!
	this->tag = "none";
	this->m_texture = new sf::Texture();
	this->transform = spe::Transform(this);
	this->m_parent_id = -1;
	this->m_id = -1;
	this->parent = nullptr;
	this->ptr_childs = std::vector<spe::Sprite*>(0);
	this->name = name;
	this->sprite_renderer.path = path;
	this->transform.SetPosition(spawnPos);

	this->sprite_renderer.sorting_layer_index = 0;

	sf::Sprite sprite;

	this->setSpriteTexture(path);

	sprite.setTexture(*this->m_texture);

	//Finally setting the sprite
	this->m_sprite = sprite;

	this->animator = spe::Animator(this);
	this->collider = spe::BoxCollider(this);
	this->physicsBody = spe::PhsysicsBody();
	this->prefab = spe::Prefab(this);
	this->light = spe::Light(this);

	this->postInit();
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