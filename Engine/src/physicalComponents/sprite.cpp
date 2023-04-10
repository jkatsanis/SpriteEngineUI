 #include "sprite.h"

//Constructor / Destructor

s2d::Sprite::Sprite() { this->name = "Unknown"; this->transform.position = Vector2(0, 0); }

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path)
{
	initVariables(name, spawnPosition, path);
}

s2d::Sprite::~Sprite()
{
	this->deleteAllChilds();
}

//Public functions

void s2d::Sprite::deleteAllChilds()
{
	this->childs.clear();
}

void s2d::Sprite::deleteChildAt(uint8_t idx)
{
	this->childs.erase(this->childs.begin() + idx);
}

void s2d::Sprite::resetChildData()
{
	this->parent->m_childCount -= 1;
	this->parent->childs.erase(parent->childs.begin() + this->m_childListPos - 1);
	this->m_parentId = -1;
	this->parent = nullptr;
	this->m_childListPos = -1;
}

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

	this->path = path;
}

void s2d::Sprite::setSpriteTexture(const std::string& path, const s2d::Vector2& scale)
{
	if (!this->m_texture.loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->m_sprite.setTexture(this->m_texture);
	this->path = path;
	this->transform.setScale(scale, true);
}

void s2d::Sprite::setParent(s2d::Sprite* parent)
{
	parent->m_childCount++;
	this->m_childListPos = parent->m_childCount + 1;
	this->parent = parent;
	this->m_parentId = parent->m_id;

	s2d::Transform::onPositionChange(this);

	parent->childs.push_back(std::make_unique<s2d::Sprite*>(this));
}

bool s2d::Sprite::containsChild(const s2d::Sprite* child) const
{
	bool contains = false;
	
	for (int i = 0; i < this->childs.size(); i++)
	{
		const s2d::Sprite* spr = *this->childs[i].get();
		if (child->m_id == spr->getId())
		{
			return true;
		}
		contains = spr->containsChild(child);
	}
	return contains;
}

s2d::Vector2 s2d::Sprite::getOrigininalPosition() const
{
	float x = this->m_sprite.getPosition().x;
	float y = this->m_sprite.getPosition().y;

	return s2d::Vector2(x, y);
}


//Private functions

void s2d::Sprite::initVariables(std::string& name, s2d::Vector2& spawnPos, std::string& path)
{
	this->validateProperties(name, spawnPos, path);
	this->transform = s2d::Transform(this);

	this->m_childCount = -1;
	this->m_childListPos = -1;
	this->m_parentId = -1;

	// ID
	s2d::SpriteData::highestSpriteID++;
	this->setId(s2d::SpriteData::highestSpriteID);

	this->parent = nullptr;
	this->childs = std::vector<std::unique_ptr<s2d::Sprite*>>(0);

	this->name = name;
	this->path = path;
	this->transform.position = spawnPos;	
	this->transform.lastPos = s2d::Vector2(0, 0);
	this->transform.nextPos = this->transform.position;
	this->sortingLayerIndex = 0;
	this->animator = s2d::Animator(this);

	sf::Sprite sprite;

	if (!this->m_texture.loadFromFile(path))
	{
		std::cout << "LOG [ERROR] Couldnt load texture from file!";
	}

	sprite.setTexture(this->m_texture);

	//Finally setting the sprite
	this->m_sprite = sprite;
	this->collider = s2d::BoxCollider(this);
	this->physicsBody = s2d::PhsysicsBody();
	this->prefab = s2d::Prefab(this);

	//Setting sprite size also in init and setTexture
	this->transform.setScale(s2d::Vector2(1, 1));
}

void s2d::Sprite::validateProperties(std::string& name, s2d::Vector2& spawnPos, std::string& path)
{
	const char CHAR_INVALID_SYMBOLS[INVALID_SPRITE_SYMBOLS] = { ';' };
	// VALIDATE NAME
	for (int i = 0; i < s2d::Sprite::s_spriteRepository->amount(); i++)
	{
		s2d::Sprite* const sprite = s2d::Sprite::s_spriteRepository->readAt(i);
		if (name == sprite->name)
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


//Static functions


void s2d::Sprite::updateHightestLayerIndex()
{
	for (int i = 0; i < s2d::Sprite::s_spriteRepository->amount(); i++)
	{
		s2d::Sprite* const sprite = s2d::Sprite::s_spriteRepository->readAt(i);
		if (sprite->sortingLayerIndex > s_highestLayerIndex)
			s_highestLayerIndex = sprite->sortingLayerIndex;
	}
}

s2d::Sprite* s2d::Sprite::getSpriteById(int id)
{
	for (int i = 0; i < s2d::Sprite::s_spriteRepository->amount(); i++)
	{
		s2d::Sprite* const sprite = s2d::Sprite::s_spriteRepository->readAt(i);
		if (sprite->m_id == id)
		{
			return sprite;
		}
	}
	return nullptr;
}

s2d::SpriteRepository* s2d::Sprite::s_spriteRepository = nullptr;
int s2d::Sprite::s_highestLayerIndex = 0;