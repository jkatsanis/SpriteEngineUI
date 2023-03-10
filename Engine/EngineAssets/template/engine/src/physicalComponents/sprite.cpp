#include "sprite.h"

//Constructor

s2d::Sprite::Sprite() { this->name = "Unknown"; this->transform.position = Vector2(0, 0); }

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path)
	: name(name)
{
	initVariables(name, spawnPosition, path);
}

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path, bool addToWindowByConstruction)
{
	initVariables(name, spawnPosition, path);

	if (addToWindowByConstruction)
	{
		this->addSpriteToScene();
	}
}

//Public functions

//////////////////////////////////////
//// USER FUNCTIONS 
/////////////////////////////////////

void s2d::Sprite::setSpriteTexture(const std::string& path)
{
	if (!this->m_texture.loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->setSpriteTexture(this->m_texture, path);
}

void s2d::Sprite::setSpriteTexture(const sf::Texture& texture, const std::string& path)
{
	this->m_sprite.setTexture(texture, true);
	this->transform.setScale(this->transform.getScale(), true);
	this->m_path = path;
}

void s2d::Sprite::setSpriteTexture(const std::string& path, const s2d::Vector2& scale)
{
	if (!this->m_texture.loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->m_sprite.setTexture(this->m_texture);
	this->m_path = path;
	this->transform.setScale(scale, true);
}

void s2d::Sprite::addSpriteToScene()
{
	this->m_vectorPosition = int(s2d::Sprite::s_sprites.size() + 1);
	s2d::Sprite::s_sprites.push_back(this);
}

s2d::Vector2 s2d::Sprite::getOrigininalPosition()
{
	float x = this->m_sprite.getPosition().x;
	float y = this->m_sprite.getPosition().y;

	return s2d::Vector2(x, y);
}

void s2d::Sprite::renderInstant()
{
	this->updateHightestLayerIndex();
}

void s2d::Sprite::updateSpriteTexture()
{
	this->setSpriteTexture(this->m_path);
}


//Private functions

void s2d::Sprite::initVariables(std::string name, s2d::Vector2 spawnPos, std::string path)
{
	this->transform = s2d::Transform(this);

	this->m_childCount = -1;
	this->m_parentId = -1;

	// ID
	s2d::SpriteData::highestSpriteID++;
	this->setId(s2d::SpriteData::highestSpriteID);

	this->parent = nullptr;
	this->childs = std::vector<s2d::Sprite*>(0);

	this->name = name;
	this->m_path = path;
	this->transform.position = spawnPos;
	this->transform.lastPos = s2d::Vector2(0, 0);
	this->transform.nextPos = this->transform.position;
	this->m_vectorPosition = -1;
	this->sortingLayerIndex = 0;

	sf::Sprite sprite;

	if (!this->m_texture.loadFromFile(path))
	{
		//Console log!
	}

	sprite.setTexture(this->m_texture);

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

int s2d::Sprite::getMaxNumber(std::vector<s2d::Sprite*>& vec)
{
	if (vec.size() == 0)
	{
		return 0;
	}
	int max = vec[0]->getVectorPosition();
	for (int i = 1; i < vec.size(); i++)
	{
		if (max < vec[i]->getVectorPosition())
		{
			max = vec[i]->getVectorPosition();
		}
	}
	return max;
}

s2d::Sprite* s2d::Sprite::getSpriteByName(std::string name)
{
	for (s2d::Sprite* sprite : s2d::Sprite::s_sprites)
	{
		if (sprite->name == name)
		{
			return sprite;
		}
	}
	return nullptr;
}

void s2d::Sprite::deleteSpriteByName(const std::string& name)
{
	// TODO: Delete child from parent 
	int deleteteAt = 0;
	for (int i = 0; i < s2d::Sprite::s_sprites.size(); i++)
	{
		if (s2d::Sprite::s_sprites[i]->name == name)
		{
			deleteteAt = i;
			//deleting the deletet sprite + freeing it
			s2d::Sprite* spr = s2d::Sprite::s_sprites[i];
	
			s2d::Sprite::s_sprites.erase((s2d::Sprite::s_sprites.begin() + spr->getVectorPosition() - 1));
			delete spr;
			spr = nullptr;
			break;
		}
	}

	for (int i = deleteteAt; i < s2d::Sprite::s_sprites.size(); i++)
	{
		s2d::Sprite::s_sprites[i]->m_vectorPosition--;
	}
}

s2d::Sprite* s2d::Sprite::getSpriteById(int id)
{
	for (s2d::Sprite* sprite : s2d::Sprite::s_sprites)
	{
		if (sprite->m_id == id)
		{
			return sprite;
		}
	}
	return nullptr;
}

void s2d::Sprite::updateHightestLayerIndex()
{
	for (const s2d::Sprite* sprite : s2d::Sprite::s_sprites)
	{
		if (sprite->sortingLayerIndex > highestLayerIndex)
			highestLayerIndex = sprite->sortingLayerIndex;
	}
}

std::vector<s2d::Sprite*> s2d::Sprite::s_sprites(0);
int s2d::Sprite::highestLayerIndex = 0;