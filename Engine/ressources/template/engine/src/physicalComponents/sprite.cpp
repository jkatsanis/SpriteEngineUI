#include "sprite.h"

//Constructor

s2d::Sprite::Sprite()
{ 
	this->name = "Unknown";
	this->transform.position = Vector2(0, 0);
	this->m_texture = new sf::Texture();
}

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path)
	: name(name)
{
	initVariables(name, spawnPosition, path);
}

s2d::Sprite::~Sprite()
{
	delete this->m_texture;
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

void s2d::Sprite::setSpriteTexture(const sf::Texture& texture, const std::string& path)
{
	this->m_sprite.setTexture(texture, true);
	this->transform.setScale(this->transform.getScale(), true);
	this->m_path = path;
}

void s2d::Sprite::setSpriteTexture(const std::string& path, const s2d::Vector2& scale)
{
	if (!this->m_texture->loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->m_sprite.setTexture(*this->m_texture);
	this->m_path = path;
	this->transform.setScale(scale, true);
}

s2d::Vector2 s2d::Sprite::getOrigininalPosition()
{
	float x = this->m_sprite.getPosition().x;
	float y = this->m_sprite.getPosition().y;

	return s2d::Vector2(x, y);
}

void s2d::Sprite::updateSpriteTexture()
{
	this->setSpriteTexture(this->m_path);
}

void s2d::Sprite::validateProperties(int id, s2d::SpriteRepository& repo)
{
	const char CHAR_INVALID_SYMBOLS[INVALID_SPRITE_SYMBOLS] = { ';' };
	// VALIDATE NAME
	for (int i = 0; i < repo.amount(); i++)
	{
		s2d::Sprite* const sprite = repo.readAt(i);
		if (this->name == sprite->name)
		{
			name = name + " (dupe) " + std::to_string(i);
			std::cout << "LOG [ERROR] Cant have duped name renamed sprite!";
		}
	}
	std::string original_name = name;

	for (int i = 0; i < INVALID_SPRITE_SYMBOLS; i++)
	{
		name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
		if (name != original_name)
		{
			std::cout << "LOG [ERROR] Cant have invalid symbol!";
		}
	}
}


//Private functions

void s2d::Sprite::initVariables(std::string name, s2d::Vector2 spawnPos, std::string path)
{
	// ID's get set in the sprite repo!!

	this->m_texture = new sf::Texture();
	this->transform = s2d::Transform(this);
	this->m_parentId = -1;

	this->parent = nullptr;
	this->ptr_childs = std::vector<s2d::Sprite*>(0);

	this->name = name;
	this->m_path = path;
	this->transform.position = spawnPos;
	this->transform.lastPos = s2d::Vector2(0, 0);
	this->transform.nextPos = this->transform.position;
	this->sortingLayerIndex = 0;

	sf::Sprite sprite;

	if (!this->m_texture->loadFromFile(path))
	{
		std::cout << "LOG [ERROR] Could not load texture from path";
	}

	sprite.setTexture(*this->m_texture);

	this->transform.setScale(this->transform.getScale());

	//Finally setting the sprite
	this->m_sprite = sprite;

	this->animator = s2d::Animator(this);
	this->collider = s2d::BoxCollider(this);
	this->physicsBody = s2d::PhsysicsBody();
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
