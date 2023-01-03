#include "sprite.h"

//Constructor

s2d::Sprite::Sprite() { this->name = "Unknown"; this->transform.position = Vector2(0, 0); }

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path)
	: name(name)
{
	//INIT GETS NOT CALLED WHEN WE READ SPRITES FROM CSV
	initVariables(name, spawnPosition, path);
}

s2d::Sprite::Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path, bool addToWindowByConstruction)
{
	//INIT GETS NOT CALLED WHEN WE READ SPRITES FROM CSV
	initVariables(name, spawnPosition, path);

	if (addToWindowByConstruction)
		s2d::Sprite::activeSprites.push_back(this);
}

//Public functions

void s2d::Sprite::setSpriteTexture(const std::string path)
{
	if (!this->m_texture.loadFromFile(path))
	{
		//No file
	}
	this->path = path;
	this->m_sprite.setTexture(this->m_texture, true);

	//Setting sprite size also in init and setTexture
	sf::Vector2u tempSize = m_texture.getSize();
	this->transform.size = s2d::Vector2(float(tempSize.x), float(tempSize.y));
}

void s2d::Sprite::addSpriteToScene()
{
	s2d::Sprite::activeSprites.push_back(this);
}

void s2d::Sprite::setSpritePosition()
{
	//Pushing the sprites from a collider if 1 exists && we collided && IF everyting is unknown (no sprite collidng) then why a check
	if (!this->collider.positionData.isEverythingUnknown())
		this->pushSetup();

	float x = 960 + this->transform.position.x - this->transform.size.x / 2;
	float y = 540 - this->transform.position.y - this->transform.size.y / 2;

	this->getSprite().setPosition(sf::Vector2f(x, y));

	this->collider.positionData.resetPosition();
	this->collider.collisionCnt = 0;

	//Setting the last position, temoporary "nextPos" needed
	if (this->transform.nextPos != this->transform.position)
	{
		this->transform.lastPos = this->transform.nextPos;
		this->transform.nextPos = this->transform.position;
	}

}

s2d::Vector2 s2d::Sprite::getOrigininalPosition()
{
	float x = this->m_sprite.getPosition().x;
	float y = this->m_sprite.getPosition().y;

	return s2d::Vector2(x, y);
}

//Private functions


void s2d::Sprite::setTextureSize()
{
	sf::Vector2u tempSize = m_texture.getSize();
	this->transform.size = Vector2(tempSize.x, tempSize.y);
}


void s2d::Sprite::initVariables(std::string name, s2d::Vector2 spawnPos, std::string path)
{
	//Giving pointer so we dont have to update it consistently
	this->collider = BoxCollider(this);

	this->name = name;
	this->path = "../Assets/assets/" + path;
	this->transform.position = spawnPos;
	this->m_vectorPosition = s2d::Sprite::getMaxNumber(s2d::Sprite::activeSprites) + 1;
	sf::Sprite sprite;

	if (!this->m_texture.loadFromFile(this->path))
	{
		//Console log!
	}

	sprite.setTexture(this->m_texture);

	//Setting sprite size also in init and setTexture
	setTextureSize();

	//Finally setting the sprite
	m_sprite = sprite;

	this->collider = BoxCollider(this);
	this->physicsBody = s2d::PhsysicsBody(this);
}

void s2d::Sprite::pushSetup()
{
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Right, false, this->transform.position.x, this->transform.nextPos.x, this->transform.lastPos.x);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Left, true, this->transform.position.x, this->transform.nextPos.x, this->transform.lastPos.x);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Down, true, this->transform.position.y, this->transform.nextPos.y, this->transform.lastPos.y);
	this->pushSpriteFromCollider(s2d::BoxColliderPositionData::Up, false, this->transform.position.y, this->transform.nextPos.y, this->transform.lastPos.y);
}

void s2d::Sprite::pushSpriteFromCollider(s2d::BoxColliderPositionData::Position p, bool smaller, float& tXY, float& lXY, float& nXY)
{
	if (!smaller)
	{
		if (this->collider.positionData.isEqual(p))
		{
			//We cant go into the gameobject when its right from us so we cant increment our x pos
			if (tXY > lXY)
			{
				//Swap lol
				tXY = lXY;
				lXY = nXY;
				nXY = tXY;
			}
		}
		return;
	}
	if (this->collider.positionData.isEqual(p))
	{
		if (tXY < lXY)
		{
			tXY = lXY;
			lXY = nXY;
			nXY = tXY;
		}
	}
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

void s2d::Sprite::initActiveSprites()
{
	//! INFO ! ALWAYS SCALE THINGS UP BY 1.5F!
	std::fstream spriteFile;

	//opening the file where all sprite data is
	spriteFile.open("..\\Engine\\saves\\sprites.txt", std::ios::in);
	if (spriteFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(spriteFile, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}

			//Splitting line
			std::string delimiter = ";";
			std::string* propertys = std::splitString(line, delimiter);

			//Creating empty sprite, then pushing it back
			Sprite* sprite = new Sprite();

			//INITIIALIZING PROPS
			sprite->name = propertys[0];
			sprite->setVectorPosition(atoi(propertys[1].c_str()));
			sprite->transform.position.x = std::stof(propertys[2].c_str());
			sprite->transform.position.y = std::stof(propertys[3].c_str());

			sprite->path = propertys[4];
			if (sprite->path == "EngineAssets/Sprites/default.png")
			{
				std::string enginePath = "..\\Engine\\";
				sprite->path = enginePath + "EngineAssets\\Sprites\\default.png";
			}


			if (!sprite->m_texture.loadFromFile(sprite->path))
			{
				//Console log!
			}
			//Setting scale
			sprite->m_sprite.setScale(sprite->m_sprite.getScale().x * s2d::GameObject::ssizeMultipliyer, sprite->m_sprite.getScale().y * s2d::GameObject::ssizeMultipliyer);

			//Setting sprite size also in init and setTexture
			sf::Vector2u tempSize = sprite->m_texture.getSize();

			//Multiplying size with 1.5f because we scale it up by 1.5f lol
			sprite->transform.size = Vector2(tempSize.x * s2d::GameObject::ssizeMultipliyer, tempSize.y * s2d::GameObject::ssizeMultipliyer);
			sprite->m_sprite.setTexture(sprite->m_texture);

			//Setting BoxCollider
			sprite->collider = BoxCollider(sprite);

			sprite->collider.boxColliderWidthLeftOrRight.x = std::stof(propertys[5].c_str()) * s2d::GameObject::ssizeMultipliyer;
			sprite->collider.boxColliderWidthLeftOrRight.y = std::stof(propertys[6].c_str()) * s2d::GameObject::ssizeMultipliyer;

			sprite->collider.boxColliderHeightUpOrDown.x = std::stof(propertys[7].c_str()) * s2d::GameObject::ssizeMultipliyer;
			sprite->collider.boxColliderHeightUpOrDown.y = std::stof(propertys[8].c_str()) * s2d::GameObject::ssizeMultipliyer;
			sprite->collider.exists = propertys[9] == "True";
			sprite->collider.isSolid = propertys[10] == "True";

			//Sorting Layer
			sprite->sortingLayerIndex = atoi(propertys[11].c_str());

			//PhysicsBody
			sprite->physicsBody.gravity = std::stof(propertys[12].c_str());
			sprite->physicsBody.mass = std::stof(propertys[13].c_str());
			sprite->physicsBody.exists = propertys[14] == "True";
			sprite->physicsBody.ptr_attachedSprite = sprite;

			//parentId, ID
			sprite->m_id = atoi(propertys[15].c_str());
			sprite->m_parentId = atoi(propertys[16].c_str());

			//Last pos, next pos
			sprite->transform.nextPos.x = std::stof(propertys[17]);
			sprite->transform.nextPos.y = std::stof(propertys[18]);

			sprite->transform.lastPos.x = std::stof(propertys[19]);
			sprite->transform.lastPos.y = std::stof(propertys[20]);

			//list pos
			sprite->m_listPos = atoi(propertys[21].c_str());
			sprite->m_childCount = atoi(propertys[22].c_str());

			//Position to parent x, and y
			sprite->transform.positionToParent.x = std::stof(propertys[23]);
			sprite->transform.positionToParent.y = std::stof(propertys[24]);

			sprite->transform.position *= s2d::GameObject::ssizeMultipliyer;
			sprite->transform.lastPos *= s2d::GameObject::ssizeMultipliyer;
			sprite->transform.nextPos *= s2d::GameObject::ssizeMultipliyer;

			sprite->transform.positionToParent.x *= s2d::GameObject::ssizeMultipliyer;
			sprite->transform.positionToParent.y *= s2d::GameObject::ssizeMultipliyer;
			//Pushing the sprite
			s2d::Sprite::activeSprites.push_back(sprite);

			//End of INITING

			//Deleting the propertys since we dont need them anymore
			delete[] propertys;
		}
	}

	//setting childs of sprites
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (sprite->m_parentId > 0)
		{
			s2d::Sprite* parent = s2d::Sprite::getSpriteById(sprite->m_parentId);
			if (parent != nullptr)
			{
				sprite->parent = parent;
				parent->childs.push_back(sprite);
			}
		}
	}
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
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (sprite->name == name)
		{
			return sprite;
		}
	}
	return nullptr;
}

s2d::Sprite* s2d::Sprite::getSpriteById(int id)
{
	for (s2d::Sprite* sprite : s2d::Sprite::activeSprites)
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
	for (const s2d::Sprite* sprite : s2d::Sprite::activeSprites)
	{
		if (sprite->sortingLayerIndex > highteLayerIndex)
			highteLayerIndex = sprite->sortingLayerIndex;
	}
}

std::vector<s2d::Sprite*> s2d::Sprite::activeSprites(0);
int s2d::Sprite::highteLayerIndex = 0;