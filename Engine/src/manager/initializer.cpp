#include "initializer.h"

// public static functions

void s2d::Initializer::initPrefab(const std::string& path, s2d::SpriteRepository& repo)
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
					sprite->parent = parent;
					parent->ptr_childs.push_back(sprite);
				}
			}
		}

		uint32_t highest = repo.highestSpriteId + 1;

		for (size_t i = 0; i < mini_repo.size(); i++)
		{
			mini_repo[i]->sprite_renderer.sorting_layer_index = 1;
			repo.add(mini_repo[i]);
		}

		s2d::SpriteRepository::setValidIds(spr, highest);

		for (size_t i = 0; i < paths_to_animations.size(); i++)
		{
			s2d::Initializer::initAnimation(paths_to_animations[i], repo, spr->getId());
		}
	}
}

void s2d::Initializer::initCamera(s2d::GUIRepository& repo)
{
	std::fstream cameraFile;

	//opening the file where all sprite data is
	cameraFile.open(PATH_TO_CAMERA_FILE, std::ios::in);
	if (cameraFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(cameraFile, line))
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
			repo.camera.transform.position.x = std::stof(propertys[0].c_str());
			repo.camera.transform.position.y = std::stof(propertys[1].c_str());
			repo.camera.camera_zoom = std::stof(propertys[2].c_str());
		}
		cameraFile.close();
	}
}
void s2d::Initializer::initAnimations(s2d::SpriteRepository& repo)
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
			s2d::Initializer::initAnimation(line, repo, -1);
		}

		knownAnimationFileStream.close();
	}
	else
	{
		std::cout << "LOG: [ERROR] could not open animation(s) file!";
	}
}

void s2d::Initializer::initSprite(const std::string& line, s2d::Sprite* sprite) 
{
	//Splitting line
	std::string delimiter = ";";
	std::vector<std::string> propertys = std::splitString(line, delimiter);

	//INITIIALIZING PROPS

# pragma region Components
	sprite->transform = s2d::Transform(sprite);
	sprite->collider = BoxCollider(sprite);
	sprite->animator = s2d::Animator(sprite);
	sprite->prefab = s2d::Prefab(sprite);

	sprite->name = propertys[0];
	sprite->transform.position.x = std::stof(propertys[2].c_str());
	sprite->transform.position.y = std::stof(propertys[3].c_str());
	sprite->sprite_renderer.path = s2d::EngineData::s_path_to_user_project + "\\" + propertys[6];

	//INFO: Setting box collider props 5 - 8 down lol

	sprite->setSpriteTexture(s2d::EngineData::s_path_to_user_project + "\\" + propertys[6],
		s2d::Vector2(s2d::Vector2(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str()))));

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
		sprite->prefab.exists = propertys[29] == "True";
		sprite->prefab.load_in_memory = propertys[30] == "True";
		sprite->prefab.engine_path_to_file = propertys[31];
		sprite->prefab.user_path_to_file = s2d::UI::getUserProjectPathSeperatetFromEnginePath(sprite->prefab.engine_path_to_file);
		sprite->prefab.path_to_old_file = sprite->prefab.engine_path_to_file;
		sprite->prefab.file_name = std::getFileOnPath(sprite->prefab.engine_path_to_file);
	#pragma endregion

		sprite->postDefaultInitialization();
}

void s2d::Initializer::initSprites(s2d::SpriteRepository& spriteRepo)
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

			//Creating empty sprite, then pushing it back
			s2d::Sprite* sprite = new Sprite();

			s2d::Initializer::initSprite(line, sprite);

			//Pushing the sprite
			spriteRepo.add(sprite);

			//End of INITING
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
				sprite->parent = parent;
				parent->ptr_childs.push_back(sprite);
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

void s2d::Initializer::initIds(uint32_t& highestId)
{
	std::fstream indexFile;
	int index = 0;

	//opening the file where all sprite data is
	indexFile.open(PATH_TO_INDEX_FILE, std::ios::in);

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
	indexFile.close();

	highestId = index;
}

// private static functions

void s2d::Initializer::initAnimation(const std::string& path, s2d::SpriteRepository& repo, uint32_t idx)
{
	bool load_idx = (idx != -1);

	std::string newPath = s2d::EngineData::s_path_to_user_project + "\\" + path;
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
				uint32_t to_load = idx;
				if (!load_idx)
				{
					to_load = std::stoi(line);
				}
				ptr_sprite = repo.getSpriteWithId(idx);
				continue;
			}
			std::vector<std::string> propertys = std::splitString(line, DELIMITER);

			frames.push_back(s2d::KeyFrame(s2d::EngineData::s_path_to_user_project + "\\" + propertys[1], atoi(propertys[0].c_str())));
		}

	}
	else std::cout << "LOG: [ERROR] could not open animation data file!";

	if (ptr_sprite != nullptr)
	{
		ptr_sprite->animator.createAnimation(animationName, path, frames);

		s2d::Animation& anim = ptr_sprite->animator.animations[animationName];
		anim.setFlagToNotDeleteAfterExit();
	}
}

