#include "sprite.h"

//Constructor

s2d::Sprite::Sprite()
{ 
	this->name = "Unknown";
	this->transform.setPosition(s2d::Vector2(0, 0));
	this->m_texture = new sf::Texture();
}

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path)
	: name(name)
{
	this->initVariables(name, spawnPosition, path);
}

s2d::Sprite::Sprite(s2d::Sprite& rhs)
{
	this->initVariables(rhs.name, rhs.transform.getPosition(), rhs.sprite_renderer.path);

	this->collider = s2d::BoxCollider(this, rhs.collider);
	this->transform = s2d::Transform(this, rhs.transform);
	this->animator = s2d::Animator(this, rhs.animator);
	this->physicsBody = s2d::PhsysicsBody(this, rhs.physicsBody);

	this->tag = rhs.tag;

	// Initing the childs
	for (size_t i = 0; i < rhs.ptr_childs.size(); i++) 
	{
		s2d::Sprite* copy_child = new s2d::Sprite(*rhs.ptr_childs[i]);
		copy_child->parent = this;
		this->ptr_childs.push_back(copy_child);
	}
}

s2d::Sprite::~Sprite()
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

void s2d::Sprite::setSpriteTexture(const std::string& path)
{
	if (!this->m_texture->loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->setSpriteTexture(*this->m_texture, path);
}

void s2d::Sprite::setSpriteTexture(const std::string& path, const s2d::Vector2& scale)
{
	if (!this->m_texture->loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->setSpriteTexture(*this->m_texture, path);
	this->transform.setScale(scale, true);
}

void s2d::Sprite::setSpriteTexture(const sf::Texture& texture, const std::string& path)
{
	this->m_sprite.setTexture(texture, true);
	this->transform.setScale(this->transform.getScale(), true);
	this->sprite_renderer.path = path;

	this->transform.setOrigin();
}

s2d::Vector2 s2d::Sprite::getOrigininalPosition()
{
	float x = this->m_sprite.getPosition().x - this->transform.texture_size.x / 2;
	float y = this->m_sprite.getPosition().y - this->transform.texture_size.y / 2;

	return s2d::Vector2(x, y);
}


void s2d::Sprite::validateProperties(int id, s2d::SpriteRepository& repo)
{
	if (repo.main_content_iniitialied)
	{
		this->m_id = id;
	}

	for (int i = 0; i < repo.amount(); i++)
	{
		s2d::Sprite* const sprite = repo.readAt(i);
		if (this->name == sprite->name)
		{
			name = name + " (D) " + std::to_string(i);
		}
	}
}

void s2d::Sprite::postInit()
{
	this->transform.setOrigin();
}

void s2d::Sprite::clearParentData()
{
	if (this->parent != nullptr)
	{
		this->parent->removeChild(this);
		this->parent = nullptr;
	}
	this->m_parent_id = 0;
}

void s2d::Sprite::setParent(s2d::Sprite* parent)
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

	s2d::Sprite* child = this;
	s2d::Vector2 distance = s2d::Vector2(parent->transform.getPosition() - child->transform.getPosition());
	child->transform.position_to_parent = distance;

	parent->ptr_childs.push_back(this);
}

void s2d::Sprite::removeChild(const s2d::Sprite* child)
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

void s2d::Sprite::initVariables(std::string name, s2d::Vector2 spawnPos, std::string path)
{
	// ID's get set in the sprite repo!!
	this->tag = "none";
	this->m_texture = new sf::Texture();
	this->transform = s2d::Transform(this);
	this->m_parent_id = -1;
	this->m_id = -1;
	this->parent = nullptr;
	this->ptr_childs = std::vector<s2d::Sprite*>(0);
	this->name = name;
	this->sprite_renderer.path = path;
	this->transform.setPosition(spawnPos);

	this->sprite_renderer.sorting_layer_index = 0;

	sf::Sprite sprite;

	this->setSpriteTexture(path);

	sprite.setTexture(*this->m_texture);	

	//Finally setting the sprite
	this->m_sprite = sprite;

	this->animator = s2d::Animator(this);
	this->collider = s2d::BoxCollider(this);
	this->physicsBody = s2d::PhsysicsBody(this);
	this->prefab = s2d::Prefab(this);
	this->light = s2d::Light(this);

	this->postInit();
}

//Static functions

s2d::Sprite* s2d::Sprite::getNode()
{
	if (this->parent == nullptr)
	{
		return this;
	}
	return parent->getNode();
}
