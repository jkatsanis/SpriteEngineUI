#include "initializer.h"

// public static functions

void s2d::Initializer::loadPrefabsInMemory()
{
	std::vector<std::string> prefab_paths;
	std::getFilePathWithExtensionInFolder("assets", EXTENSION_PREFAB_FILE, prefab_paths);

	for (size_t i = 0; i < prefab_paths.size(); i++)
	{
		s2d::Initializer::initPrefab(prefab_paths[i]);
	}
}

void s2d::Initializer::initBackground(s2d::Vector3& vec)
{
	std::fstream backgroundFile;

	//opening the file where all sprite data is
	backgroundFile.open(PATH_TO_BACKGROUND, std::ios::in);
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

void s2d::Initializer::initScenes(std::vector<std::string>& scenes)
{
	std::fstream scene_file;

	scene_file.open(PATH_TO_SCENES_FILE, std::ios::in);
	if (scene_file.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(scene_file, line))
		{
			cnt++;
			if (cnt == 1)
			{
				continue;
			}
			const std::string path = PATH_TO_SAVES_FOLDER + std::string("\\") + line;
			if (std::filesystem::exists(path))
			{
				scenes.push_back(line);
			}
		}

		scene_file.close();
	}

	if (scenes.size() > 0)
	{
		s2d::GameData::s_scene = scenes[0];
	}
	else
	{
		s2d::GameData::s_scene = USER_FIRST_SCENE_NAME;
		scenes.push_back(USER_FIRST_SCENE_NAME);
	}
}

void s2d::Initializer::initAnimations(s2d::SpriteRepository& repo)
{
	std::fstream knownAnimationFileStream;

	knownAnimationFileStream.open(PATH_TO_ANIMATION);

	if (knownAnimationFileStream.is_open())
	{ 
		std::string line;
		int cnt = 0;
		while (std::getline(knownAnimationFileStream, line))
		{
			cnt++;
			if (cnt == 1) continue;
			s2d::Initializer::initAnimation(line, repo);
		}

		knownAnimationFileStream.close();
	}
	else std::cout << "LOG: [ERROR] could not open animation(s) file!";
}

void s2d::Initializer::initSprites(s2d::SpriteRepository& spriteRepo)
{
	//! INFO ! ALWAYS SCALE THINGS UP BY 1.5F!
	std::fstream spriteFile;
	std::string path = PATH_TO_SPRITES;
	//opening the file where all sprite data is
	spriteFile.open(PATH_TO_SPRITES, std::ios::in);
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

			//Creating empty sprite, then pushing it back
			Sprite* sprite = new Sprite();

			//INITIIALIZING PROPS
			s2d::Initializer::initSprite(line, sprite);
		
			sprite->postInit();

			//Pushing the sprite
			spriteRepo.add(sprite);
		}
	}

	spriteFile.close();

	//setting childs of sprites
	for (int i = 0; i < spriteRepo.amount(); i++)
	{
		s2d::Sprite* const sprite = spriteRepo.readAt(i);
		if (sprite->getParentId() > 0)
		{
			s2d::Sprite* parent = spriteRepo.getSpriteWithId(sprite->getParentId());
			if (parent != nullptr)
			{
				sprite->setParent(parent);
			}
		}
	}

}

void s2d::Initializer::initIds(uint32_t& highestId)
{
	std::fstream indexFile;
	int index = 0;

	//opening the file where all sprite data is
	indexFile.open(PATH_TO_INDEX, std::ios::in);
	if (indexFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(indexFile, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}

			index = atoi(line.c_str());
		}
	}
	highestId = index;
}

// private static functions

void s2d::Initializer::initPrefab(const std::string& path)
{
	std::fstream stream = std::fstream(path);

	if (stream.is_open())
	{
		s2d::Sprite* spr = new s2d::Sprite();
		std::vector<s2d::Sprite*> mini_repo;
		std::vector<std::string> paths_to_animations;
		std::string line = "";
		uint8_t cnt = 0;
		while (getline(stream, line))
		{
			cnt++;
			//First line is the header so we dont need to check for it
			if (cnt == 1)
			{
				continue;
			}
			if (cnt == 2)
			{
				s2d::Initializer::initSprite(line, spr);
			}

			// 'a' stands for the starts with assets 
			if (cnt > 2 && line[0] != 'a')
			{
				s2d::Sprite* child = new Sprite();
				s2d::Initializer::initSprite(line, child);
				mini_repo.push_back(child);
			}
			if (cnt > 2 && line[0] == 'a')
			{
				paths_to_animations.push_back(line);
			}
		}
		spr->parent = nullptr;
		spr->setParentId(-1);
		mini_repo.push_back(spr);

		for (int i = 0; i < mini_repo.size(); i++)
		{
			s2d::Sprite* const sprite = mini_repo[i];
			if (sprite->getParentId() > 0)
			{
				s2d::Sprite* parent = s2d::SpriteRepository::getWithId(mini_repo, sprite->getParentId());
				if (parent != nullptr)
				{
					sprite->setParent(parent);
				}
			}
		}

		for (size_t i = 0; i < paths_to_animations.size(); i++)
		{
			s2d::SpriteRepository repo;

			s2d::Initializer::initAnimation(paths_to_animations[i], spr);
		}

		spr->setId(-1);
		// TODO: VALID CHILD IDS
		if (spr->prefab.load_in_memory)
		{
			s2d::PrefabRepositor::addPrefab(spr);
		}
		else
		{
			delete spr;
			spr = nullptr;
		}	
	}
}

void s2d::Initializer::initAnimation(const std::string& path, s2d::SpriteRepository& repo)
{
	std::fstream animationFileStream;
	animationFileStream.open(path);
	s2d::Sprite* ptr_sprite = nullptr;

	if (animationFileStream.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(animationFileStream, line))
		{
			cnt++;
			if (cnt == 1)
			{
				continue;
			}
			if (cnt == 2)
			{
				uint32_t idx = std::stoi(line);
				ptr_sprite = repo.getSpriteWithId(idx);
				break;
			}
		}
		animationFileStream.close();
	}
	if (ptr_sprite != nullptr)
	{
		s2d::Initializer::initAnimation(path, ptr_sprite);
	}
}

void s2d::Initializer::initAnimation(const std::string& path, s2d::Sprite* spr)
{
	std::fstream animationFileStream;

	animationFileStream.open(path);

	s2d::Sprite* ptr_sprite = spr;
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
			if(cnt == 2)
			{
				continue;
			}

			std::vector<std::string> propertys = std::splitString(line, DELIMITER);

			if (propertys[1] != "")
				frames.push_back(s2d::KeyFrame(propertys[1], std::stof(propertys[0].c_str())));
		}
		animationFileStream.close();
	}
	else std::cout << "LOG: [ERROR] could not open animation data file!";
	if (ptr_sprite != nullptr)
	{
		ptr_sprite->animator.createAnimation(animationName, path, frames);
	}
}

void s2d::Initializer::initSprite(const std::string& line, s2d::Sprite* sprite)
{
	//Splitting line
	std::string delimiter = ";";
	std::vector<std::string> propertys = std::splitString(line, delimiter);

	sprite->collider = s2d::BoxCollider(sprite);
	sprite->animator = s2d::Animator(sprite);
	sprite->transform = s2d::Transform(sprite);
	sprite->physicsBody = s2d::PhsysicsBody(sprite);
	sprite->prefab = s2d::Prefab(sprite);

	sprite->name = propertys[0];
	sprite->transform.position.x = std::stof(propertys[2].c_str());
	sprite->transform.position.y = std::stof(propertys[3].c_str());

	sprite->setSpriteTexture(propertys[6], s2d::Vector2(s2d::Vector2(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str()))));

	sprite->transform.setRotation(atoi(propertys[7].c_str()));

# pragma region Collider
	sprite->collider.box_collider_width.x = std::stof(propertys[8].c_str());
	sprite->collider.box_collider_width.y = std::stof(propertys[9].c_str());

	sprite->collider.box_collider_height.x = std::stof(propertys[10].c_str());
	sprite->collider.box_collider_height.y = std::stof(propertys[11].c_str());
	sprite->collider.exist = propertys[12] == "True";
	sprite->collider.is_solid = propertys[13] == "True";

#pragma endregion
# pragma region Sorting Layer
	sprite->sprite_renderer.sorting_layer_index = atoi(propertys[14].c_str());


#pragma endregion
# pragma region PhysicsBody
	sprite->physicsBody.gravity = std::stof(propertys[15].c_str());
	sprite->physicsBody.mass = std::stof(propertys[16].c_str());
	sprite->physicsBody.exist = propertys[17] == "True";

#pragma endregion
# pragma region parentId, ID
	sprite->setId(atoi(propertys[18].c_str()));
	sprite->setParentId(atoi(propertys[19].c_str()));

#pragma endregion
# pragma region Last pos, next pos
	sprite->transform.next_pos.x = std::stof(propertys[20]);
	sprite->transform.next_pos.y = std::stof(propertys[21]);

	sprite->transform.last_pos.x = std::stof(propertys[22]);
	sprite->transform.last_pos.y = std::stof(propertys[23]);

#pragma endregion

# pragma region Position to parent x, and y
	sprite->transform.position_to_parent.x = std::stof(propertys[26]);
	sprite->transform.position_to_parent.y = std::stof(propertys[27]);

	sprite->animator.exist = propertys[28] == "True";
#pragma endregion

# pragma region Prefab
	sprite->prefab.exist = propertys[29] == "True";
	sprite->prefab.load_in_memory = propertys[30] == "True";
#pragma endregion
}