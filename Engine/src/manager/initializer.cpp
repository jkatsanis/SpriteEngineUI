#include "initializer.h"

// public static functions

void s2d::Initializer::initAnimations()
{
	std::fstream knownAnimationFileStream;

	knownAnimationFileStream.open(PATH_TO_KNOWN_ANIMATIONS);

	if (knownAnimationFileStream.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(knownAnimationFileStream, line))
		{
			cnt++;
			if (cnt == 1) continue;
			s2d::Initializer::initAnimation(line);
		}

		knownAnimationFileStream.close();
	}
	else
	{
		std::cout << "LOG: [ERROR] could not open animation(s) file!";
	}
}

void s2d::Initializer::initSprites()
{
	std::fstream spriteFile;

	//opening the file where all sprite data is

	spriteFile.open(PATH_TO_SPRITE_FILE, std::ios::in);
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

			// Components
			sprite->transform = s2d::Transform(sprite);
			sprite->collider = BoxCollider(sprite);
			sprite->animator = s2d::Animator(sprite);
			sprite->prefab = s2d::Prefab(sprite);

			sprite->name = propertys[0];
			sprite->setVectorPosition(atoi(propertys[1].c_str()));
			sprite->transform.position.x = std::stof(propertys[2].c_str());
			sprite->transform.position.y = std::stof(propertys[3].c_str());
			sprite->path = s2d::EngineData::s_pathToUserProject + "\\" + propertys[6];

			//INFO: Setting box collider props 5 - 8 down lol

			sprite->setSpriteTexture(s2d::EngineData::s_pathToUserProject + "\\" + propertys[6], 
				s2d::Vector2(s2d::Vector2(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str()))));

			//Collider
			sprite->collider.boxColliderWidthLeftOrRight.x = std::stof(propertys[7].c_str());
			sprite->collider.boxColliderWidthLeftOrRight.y = std::stof(propertys[8].c_str());

			sprite->collider.boxColliderHeightUpOrDown.x = std::stof(propertys[9].c_str());
			sprite->collider.boxColliderHeightUpOrDown.y = std::stof(propertys[10].c_str());
			sprite->collider.exists = propertys[11] == "True";
			sprite->collider.isSolid = propertys[12] == "True";

			//Sorting Layer
			sprite->sortingLayerIndex = atoi(propertys[13].c_str());

			//PhysicsBody
			sprite->physicsBody.gravity = std::stof(propertys[14].c_str());
			sprite->physicsBody.mass = std::stof(propertys[15].c_str());
			sprite->physicsBody.exists = propertys[16] == "True";

			//parentId, ID
			sprite->setId(atoi(propertys[17].c_str()));
			sprite->setParentId(atoi(propertys[18].c_str()));

			//Last pos, next pos
			sprite->transform.nextPos.x = std::stof(propertys[19]);
			sprite->transform.nextPos.y = std::stof(propertys[20]);

			sprite->transform.lastPos.x = std::stof(propertys[21]);
			sprite->transform.lastPos.y = std::stof(propertys[22]);

			//list pos
			sprite->setChildListPos(atoi(propertys[23].c_str()));
			sprite->setChildCount(atoi(propertys[24].c_str()));

			//Position to parent x, and y
			sprite->transform.positionToParent.x = std::stof(propertys[25]);
			sprite->transform.positionToParent.y = std::stof(propertys[26]);

			sprite->animator.exists = propertys[27] == "True";

			sprite->prefab.exists = propertys[28] == "True";
			if (sprite->prefab.exists)
			{
				sprite->prefab.fileName = std::getFileOnPath(propertys[30]);
				sprite->prefab.loadInMemory = propertys[29] == "True";
				sprite->prefab.enginePathToFile = propertys[30];
				sprite->prefab.pathToOldFile = propertys[30]; // Same
				sprite->prefab.userPathToFile = s2d::UI::getUserProjectPathSeperatetFromEnginePath(propertys[30]);
			}
			else
			{
				sprite->prefab.resetPrefab();
			}

			//Pushing the sprite
			s2d::Sprite::s_sprites.push_back(sprite);

			//End of INITING
		}
	}

	spriteFile.close();

	//setting childs of sprites
	for (s2d::Sprite* sprite : s2d::Sprite::s_sprites)
	{
		if (sprite->getParentId() > 0)
		{
			s2d::Sprite* parent = s2d::Sprite::getSpriteById(sprite->getParentId());
			if (parent != nullptr)
			{
				sprite->parent = parent;
				parent->childs.push_back(sprite);
			}
		}
	}
}

void s2d::Initializer::initBackground(s2d::Vector3& vec)
{
	std::fstream backgroundFile;

	//opening the file where all sprite data is
	backgroundFile.open(PATH_TO_BACKGROUND_FILE, std::ios::in);
	if (backgroundFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(backgroundFile, line))
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

			//INITIIALIZING PROPS
			vec.x = std::stof(propertys[0].c_str());
			vec.y = std::stof(propertys[1].c_str());
			vec.z = std::stof(propertys[2].c_str());
		}
		backgroundFile.close();

	}
}


// private static functions

void s2d::Initializer::initAnimation(const std::string& path)
{
	std::string newPath = s2d::EngineData::s_pathToUserProject + "\\" + path;
	std::fstream animationFileStream;

	animationFileStream.open(newPath);

	s2d::Sprite* ptr_sprite = nullptr;
	std::string animationName = "";

	std::vector<s2d::KeyFrame> frames = std::vector<s2d::KeyFrame>(0);

	if (animationFileStream.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(animationFileStream, line))
		{
			cnt++;
			if (cnt == 1)
			{
				animationName = line;
				continue;
			}
			if (cnt == 2)
			{
				// read only the ID of the sprite to apply the animtion
				int idx = std::stoi(line);
				ptr_sprite = s2d::Sprite::getSpriteById(idx);
				continue;
			}
			std::vector<std::string> propertys = std::splitString(line, DELIMITER);

			frames.push_back(s2d::KeyFrame(s2d::EngineData::s_pathToUserProject + "\\" + propertys[1], atoi(propertys[0].c_str())));
		}

	}
	else std::cout << "LOG: [ERROR] could not open animation data file!";

	if (ptr_sprite != nullptr)
	{
		ptr_sprite->animator.createAnimation(animationName, path, frames);
	}
}

