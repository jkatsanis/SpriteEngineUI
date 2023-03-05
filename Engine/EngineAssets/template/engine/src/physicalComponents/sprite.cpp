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

void s2d::Sprite::setSpriteTexture(const std::string path)
{
	if (!this->m_texture.loadFromFile(path))
	{
		std::cout << "LOG: [ERROR] File was not found!";
	}
	this->m_path = path;
	this->m_sprite.setTexture(this->m_texture, true);
	this->transform.setScale(this->transform.getScale(), true);
}

void s2d::Sprite::addSpriteToScene()
{
	this->m_vectorPosition = int(s2d::Sprite::activeSprites.size() + 1);
	s2d::Sprite::activeSprites.push_back(this);
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

void s2d::Sprite::initActiveSprites()
{
	//! INFO ! ALWAYS SCALE THINGS UP BY 1.5F!
	std::fstream spriteFile;


	//opening the file where all sprite data is
	spriteFile.open("engine\\saves\\sprites.txt", std::ios::in);
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
			std::vector<std::string> propertys = std::splitString(line, delimiter);

			//Creating empty sprite, then pushing it back
			Sprite* sprite = new Sprite();

			//INITIIALIZING PROPS

			sprite->transform = s2d::Transform(sprite);

			sprite->name = propertys[0];
			sprite->setVectorPosition(atoi(propertys[1].c_str()));
			sprite->transform.position.x = std::stof(propertys[2].c_str());
			sprite->transform.position.y = std::stof(propertys[3].c_str());
			sprite->m_path = propertys[6];

			if (!sprite->m_texture.loadFromFile(sprite->m_path))
			{
				//Console log!
			}

			//Setting sprite size also in init and setTexture
			sf::Vector2u tempSize = sprite->m_texture.getSize();

			//Multiplying size with 1.5f because we scale it up by 1.5f lol
			sprite->transform.textureSize = Vector2(tempSize.x * s2d::GameObject::ssizeMultipliyer, tempSize.y * s2d::GameObject::ssizeMultipliyer);
			sprite->m_sprite.setTexture(sprite->m_texture);

			sprite->transform.setScale(s2d::Vector2(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str())));

			//Setting BoxCollider
			sprite->collider = BoxCollider(sprite);

			sprite->collider.boxColliderWidthLeftOrRight.x = std::stof(propertys[7].c_str()) * s2d::GameObject::ssizeMultipliyer;
			sprite->collider.boxColliderWidthLeftOrRight.y = std::stof(propertys[8].c_str()) * s2d::GameObject::ssizeMultipliyer;

			sprite->collider.boxColliderHeightUpOrDown.x = std::stof(propertys[9].c_str()) * s2d::GameObject::ssizeMultipliyer;
			sprite->collider.boxColliderHeightUpOrDown.y = std::stof(propertys[10].c_str()) * s2d::GameObject::ssizeMultipliyer;
			sprite->collider.exists = propertys[11] == "True";
			sprite->collider.isSolid = propertys[12] == "True";

			//Sorting Layer
			sprite->sortingLayerIndex = atoi(propertys[13].c_str());

			//PhysicsBody
			sprite->physicsBody.gravity = std::stof(propertys[14].c_str());
			sprite->physicsBody.mass = std::stof(propertys[15].c_str());
			sprite->physicsBody.exists = propertys[16] == "True";
			sprite->physicsBody.ptr_attachedSprite = sprite;

			//parentId, ID
			sprite->m_id = atoi(propertys[17].c_str());
			sprite->m_parentId = atoi(propertys[18].c_str());

			//Last pos, next pos
			sprite->transform.nextPos.x = std::stof(propertys[19]);
			sprite->transform.nextPos.y = std::stof(propertys[20]);

			sprite->transform.lastPos.x = std::stof(propertys[21]);
			sprite->transform.lastPos.y = std::stof(propertys[22]);

			//list pos
			sprite->m_childListPos = atoi(propertys[23].c_str());
			sprite->m_childCount = atoi(propertys[24].c_str());

			//Position to parent x, and y
			sprite->transform.positionToParent.x = std::stof(propertys[25]);
			sprite->transform.positionToParent.y = std::stof(propertys[26]);

			sprite->transform.position *= s2d::GameObject::ssizeMultipliyer;
			sprite->transform.lastPos *= s2d::GameObject::ssizeMultipliyer;
			sprite->transform.nextPos *= s2d::GameObject::ssizeMultipliyer;

			sprite->transform.positionToParent.x *= s2d::GameObject::ssizeMultipliyer;
			sprite->transform.positionToParent.y *= s2d::GameObject::ssizeMultipliyer;

			//Pushing the sprite
			s2d::Sprite::activeSprites.push_back(sprite);
		}
	}

	spriteFile.close();

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

void s2d::Sprite::deleteSpriteByName(const std::string& name)
{
	// TODO: Delete child from parent 
	int deleteteAt = 0;
	for (int i = 0; i < s2d::Sprite::activeSprites.size(); i++)
	{
		if (s2d::Sprite::activeSprites[i]->name == name)
		{
			deleteteAt = i;
			//deleting the deletet sprite + freeing it
			s2d::Sprite* spr = s2d::Sprite::activeSprites[i];
	
			s2d::Sprite::activeSprites.erase((s2d::Sprite::activeSprites.begin() + spr->getVectorPosition() - 1));
			delete spr;
			spr = nullptr;
			break;
		}
	}

	for (int i = deleteteAt; i < s2d::Sprite::activeSprites.size(); i++)
	{
		s2d::Sprite::activeSprites[i]->m_vectorPosition--;
	}
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
		if (sprite->sortingLayerIndex > highestLayerIndex)
			highestLayerIndex = sprite->sortingLayerIndex;
	}
}

std::vector<s2d::Sprite*> s2d::Sprite::activeSprites(0);
int s2d::Sprite::highestLayerIndex = 0;