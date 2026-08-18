#include "Initializer.h"
#include "Core/SeceneHandler.h"

// public static functions

#pragma region SPRITE

void spe::Initializer::InitSprites(spe::SpriteRepository& spriteRepo, const std::string& path, spe::LightRepository& lightrepo)
{
	std::fstream spriteFile;
	OPEN_FILE(spriteFile, path, std::ios::in);

	if (spriteFile.is_open())
	{
		spe::Sprite* current_sprite = nullptr;

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

			const std::vector<std::string> properties = spe::Utility::Split(line, PREFAB_DELIMITER);

			// S stands for sprite
			if (properties[0] == "S")
			{
				spe::Sprite* child = spe::Initializer::InitSprite(properties[1], lightrepo);
				current_sprite = child;
				spriteRepo.Add(child);
			}

			// A stands for animation
			if (properties[0] == "A")
			{
				spe::Initializer::InitAnimation(properties[1], current_sprite);
			}

			// M stands for music (even though its audio component, but A was already taken)
			if (properties[0] == "M")
			{
				InitAudio(properties[1], current_sprite);
			}

			// P stands for particles
			if (properties[0] == "P")
			{
				InitParticles(properties[1], current_sprite);
			}
		}
	}

	spriteFile.close();

	// Algorithm to set childs of the sprite
	std::list<spe::Sprite*>& sprites = spriteRepo.GetSprites();

	for (auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		spe::Sprite* sprite = *it;
		if (sprite->GetParentId() > 0)
		{
			if (!spriteRepo.ExistWithId(sprite->GetParentId()))
			{
				spe::Log::LogString("Sprite parent not found!");
				sprite->ClearParentData();
				continue;
			}
			spe::Sprite* parent = spriteRepo.GetById(sprite->GetParentId());
			if (parent != nullptr)
			{
				sprite->SetParent(parent);
			}
		}

	}
}


void spe::Initializer::InitAnimation(const std::string& path, spe::Sprite* spr)
{
	std::string updatetPath = path;
#ifdef __linux__
	updatetPath = spe::Utility::ToLinuxPath(path);
#else
	updatetPath = spe::Utility::ToWindowsPath(path);
#endif


	const std::string ext = "." + spe::Utility::GetFileExtension(updatetPath);
	if (ext != EXTENSION_ANIMATION_FILE)
	{
		throw std::runtime_error("Animation path file isnt �n the right format");
		return;
	}

	std::fstream animationFileStream;

	animationFileStream.open(updatetPath);

	spe::Sprite* ptr_sprite = spr;
	std::string animationName = "";

	std::vector<spe::KeyFrame> frames = std::vector<spe::KeyFrame>(0);
	bool loop = false;
	bool playOnStart = false;
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
				// Skipping the sprite get line
				continue;
			}
			if (cnt == 3)
			{
				loop = line == "True";
				continue;
			}
			if (cnt == 4
				&& !std::isdigit(static_cast<unsigned char>(line[0])))
			{
				playOnStart = line == "True";
				continue;
			}
			std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

			if (!propertys.empty())
			{
				const float delay = std::stof(propertys[0].c_str());
				frames.push_back(spe::KeyFrame(propertys[1], delay));
			}
		}
	}
	else
	{
		spe::Log::LogString("Couldnt open animation file!");
	}

	if (ptr_sprite != nullptr)
	{
		ptr_sprite->Animator.CreateAnimation(animationName, updatetPath, frames);

		spe::Animation& anim = ptr_sprite->Animator.Animations[animationName];
		anim.Loop = loop;
		anim.PlayOnStart = playOnStart;
	}
}

spe::Sprite* spe::Initializer::InitSprite(const std::string& line, spe::LightRepository& lightrepo)
{
	std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

	const spe::Vector2 position = spe::Vector2(std::stof(propertys[2].c_str()), std::stof(propertys[3].c_str()));

	spe::Sprite* sprite = new spe::Sprite(propertys[0], position, propertys[6], lightrepo);

	spe::Vector2 vec(std::stof(propertys[4].c_str()), std::stof(propertys[5].c_str()));
	sprite->Transform.SetScale(vec, true);

	sprite->Transform.SetRotation(atoi(propertys[7].c_str()));

# pragma region Collider
	sprite->Collider.Width.X = std::stof(propertys[8].c_str());
	sprite->Collider.Width.Y = std::stof(propertys[9].c_str());

	sprite->Collider.Height.X = std::stof(propertys[10].c_str());
	sprite->Collider.Height.Y = std::stof(propertys[11].c_str());
	sprite->Collider.Exist = propertys[12] == "True";
	sprite->Collider.IsSolid = propertys[13] == "True";

#pragma endregion
# pragma region Sorting Layer
	sprite->SpriteRenderer.SortinLayerIdx = atoi(propertys[14].c_str());


#pragma endregion
# pragma region PhysicsBody
	sprite->Physicsbody.Gravity = std::stof(propertys[15].c_str());
	sprite->Physicsbody.Mass = std::stof(propertys[16].c_str());
	sprite->Physicsbody.Exist = propertys[17] == "True";

#pragma endregion
# pragma region parentId, ID
	sprite->SetId(atoi(propertys[18].c_str()));
	sprite->SetParentId(atoi(propertys[19].c_str()));

#pragma endregion
# pragma region Last pos, next pos


#pragma endregion

# pragma region Position to parent x, and y
	sprite->Transform.PositionToParent.X = std::stof(propertys[26]);
	sprite->Transform.PositionToParent.Y = std::stof(propertys[27]);

	sprite->Animator.Exist = propertys[28] == "True";
#pragma endregion

# pragma region Prefab
	sprite->Prefab.Exist = propertys[29] == "True";
	sprite->Prefab.LoadInMemory = propertys[30] == "True";
	sprite->Prefab.PathToFile = propertys[31];
	sprite->Prefab.UpdateName();
	sprite->Prefab.UpdatePath();
#pragma endregion

#pragma region General
	sprite->Tag = propertys[32];
#pragma endregion


#pragma region Light
	sprite->Light.SetRadius(std::stof(propertys[34]));
	sprite->Light.SetIntensity(std::stof(propertys[35]));
	if (propertys[33] == "True")
	{
		sprite->Light.Enable();
	}

	sprite->SpriteRenderer.EffectedByLight = propertys[36] == "True";

	spe::Vector3 color;

	color.X = std::stof(propertys[37]);
	color.Y = std::stof(propertys[38]);
	color.Z = std::stof(propertys[39]);

	sprite->Light.SetColor(spe::Vector3::ToSFVector3(color));
#pragma endregion

#pragma region render
	sprite->SpriteRenderer.Render = propertys[40] == "True";
#pragma endregion

#pragma region PhysicsBody-Friction
	if (propertys.size() >= 42)
	{
		sprite->Physicsbody.Friction = std::stof(propertys[41]);
	}

	if (propertys.size() >= 43)
	{
		sprite->Physicsbody.UseAirFriction = propertys[42] == "True";
	}
#pragma endregion

#pragma region Collider-Performance
	if (propertys.size() >= 44)
	{
		sprite->Collider.Controller = propertys[43] == "True";
	}
#pragma endregion
	sprite->Transform.SetOrigin();

	return sprite;
}

#pragma endregion


#pragma region CAMERA

void spe::Initializer::InitCamera(spe::Camera& camera, const std::string& path)
{
	std::fstream cameraFile;

	//opening the file where all sprite data is
	cameraFile.open(path, std::ios::in);
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
			std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

			//INITIIALIZING PROPS
			const spe::Vector2 position(std::stof(propertys[0].c_str()), std::stof(propertys[1].c_str()));
		
			camera.Position = position;
			camera.SetZoom(std::stof(propertys[2].c_str()));
			camera.CameraSpeed = std::stof(propertys[3].c_str());
		}
		cameraFile.close();
	}
}

void spe::Initializer::InitAudio(const std::string &line, spe::Sprite *spr) {
	const std::vector<std::string> properties = spe::Utility::Split(line, DELIMITER);
	const std::string& name = properties[0];
	const std::string path = Utility::ToRightPath(properties[1]);
	const std::string& type = properties[2];
	const std::string& playOnStart = properties[3];
	const std::string& volume = properties[4];
	const std::string& pitch = properties[5];
	const std::string& relative = properties[6];
	const std::string& minDistance = properties[7];
	const std::string& attenuation = properties[8];
	const std::string& loop = properties[9];
	const std::string& loopPoints = properties[10];

	if (spr == nullptr) {
		return;
	}

	if (!spr->Audio.Exist) {
		spr->Audio.Exist = true; // init Audio comp, in case it isn't yet
	}

	Audio* audio = new Audio();
	Audio::AudioType aType;
	if (type == "Sound") {
		aType = Audio::AudioType::SOUND;
	}
	else if (type == "Music") {
		aType = Audio::AudioType::MUSIC;
	}
	else {
		throw std::runtime_error("Audio type not found");
	}
	audio->Configure(path, aType, name);
	audio->SetVolume(std::stof(volume));
	audio->SetPitch(std::stof(pitch));
	audio->PlayOnStart = playOnStart == "True";

	audio->SetLoop(loop == "True");
	if (aType == Audio::AudioType::SOUND) {
		audio->SetAttenuation(std::stof(attenuation));
		audio->SetMinDistance(std::stof(minDistance));
		audio->SetRelativeToListener(relative == "True");
		const Vector2 position = spr->Transform.GetPosition();
		audio->SetPosition(position.X, position.Y);
	}
	else if (loopPoints != "-1,-1") { // only available for MUSIC
		const float offset = std::stof(loopPoints.substr(0, loopPoints.find(',')));
		const float length = std::stof(loopPoints.substr(loopPoints.find(',') + 1));
		audio->SetLoopPoints(offset, length);
	}
	spr->Audio.Audios.push_back(audio);

}

void spe::Initializer::InitParticles(const std::string& line, spe::Sprite* spr)
{
	if (spr == nullptr)
	{
		return;
	}

	const std::vector<std::string> properties = spe::Utility::Split(line, DELIMITER);

	spe::ParticleEmitterConfig config;

	int i = 0;

	// PlayOnStart
	config.PlayOnStart = properties[i++] == "True";

	// Emission settings
	config.EmissionRate = std::stof(properties[i++]);
	config.MaxParticles = static_cast<uint32_t>(std::stoi(properties[i++]));
	config.Shape = static_cast<spe::EmissionShape>(std::stoi(properties[i++]));
	config.ShapeRadius = std::stof(properties[i++]);
	config.ShapeSize.X = std::stof(properties[i++]);
	config.ShapeSize.Y = std::stof(properties[i++]);
	config.LineAngle = std::stof(properties[i++]);

	// Lifetime and speed
	config.LifetimeMin = std::stof(properties[i++]);
	config.LifetimeMax = std::stof(properties[i++]);
	config.SpeedMin = std::stof(properties[i++]);
	config.SpeedMax = std::stof(properties[i++]);

	// Direction
	config.Direction.X = std::stof(properties[i++]);
	config.Direction.Y = std::stof(properties[i++]);
	config.DirectionSpread = std::stof(properties[i++]);

	// Offset
	config.Offset.X = std::stof(properties[i++]);
	config.Offset.Y = std::stof(properties[i++]);

	// Start color (RGBA)
	config.StartColorMin.r = static_cast<sf::Uint8>(std::stoi(properties[i++]));
	config.StartColorMin.g = static_cast<sf::Uint8>(std::stoi(properties[i++]));
	config.StartColorMin.b = static_cast<sf::Uint8>(std::stoi(properties[i++]));
	config.StartColorMin.a = static_cast<sf::Uint8>(std::stoi(properties[i++]));
	config.StartColorMax = config.StartColorMin;

	// End color (RGBA)
	config.EndColorMin.r = static_cast<sf::Uint8>(std::stoi(properties[i++]));
	config.EndColorMin.g = static_cast<sf::Uint8>(std::stoi(properties[i++]));
	config.EndColorMin.b = static_cast<sf::Uint8>(std::stoi(properties[i++]));
	config.EndColorMin.a = static_cast<sf::Uint8>(std::stoi(properties[i++]));
	config.EndColorMax = config.EndColorMin;

	// Size
	config.StartSizeMin = std::stof(properties[i++]);
	config.StartSizeMax = std::stof(properties[i++]);
	config.EndSizeMin = std::stof(properties[i++]);
	config.EndSizeMax = std::stof(properties[i++]);

	// Rotation
	config.RotationMin = std::stof(properties[i++]);
	config.RotationMax = std::stof(properties[i++]);
	config.RotationSpeedMin = std::stof(properties[i++]);
	config.RotationSpeedMax = std::stof(properties[i++]);

	// Physics
	config.Gravity.X = std::stof(properties[i++]);
	config.Gravity.Y = std::stof(properties[i++]);
	config.Drag = std::stof(properties[i++]);

	// Blend mode
	bool isAdditive = properties[i++] == "True";
	config.BlendMode = isAdditive ? sf::BlendAdd : sf::BlendAlpha;

	// Follow sprite
	bool followSprite = properties[i++] == "True";

	// Texture path
	std::string texturePath = "";
	if (i < properties.size())
	{
		texturePath = properties[i++];
		// Handle "none" or empty, and guard against boolean strings that might indicate format mismatch
		if (texturePath == "none" || texturePath == "True" || texturePath == "False" || texturePath.empty())
		{
			texturePath = "";
		}
	}

	// Apply config to sprite
	spr->Particles.Exist = true;
	spr->Particles.GetParticleSystem().SetConfig(config);
	spr->Particles.SetFollowSprite(followSprite);
	if (!texturePath.empty())
	{
		spr->Particles.SetTexturePath(texturePath);
	}
}

void spe::Initializer::InitGeneralSettings(const std::string& path)
{
	std::fstream backgroundFile;

	//opening the file where all sprite data is
	backgroundFile.open(path, std::ios::in);
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
			std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

			//INITIIALIZING PROPS
			spe::EngineData::s_JumpThroughBoxes = propertys[0] == "True";
		}
		backgroundFile.close();
	}
}

#pragma endregion

#pragma region BACKGROUND

void spe::Initializer::InitBackground(spe::Vector3& vec, const std::string& path)
{
	std::fstream backgroundFile;

	//opening the file where all sprite data is
	backgroundFile.open(path, std::ios::in);
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
			std::vector<std::string> propertys = spe::Utility::Split(line, DELIMITER);

			//INITIIALIZING PROPS
			vec.X = std::stof(propertys[0].c_str());
			vec.Y = std::stof(propertys[1].c_str());
			vec.Z = std::stof(propertys[2].c_str());
		}
		backgroundFile.close();
	}
}

#pragma endregion

spe::Sprite* spe::Initializer::InitPrefab(const std::string& path, spe::LightRepository& repo)
{
	std::fstream stream;
	OPEN_FILE(stream, path);

	if (stream.is_open())
	{
		if (stream.peek() == std::ifstream::traits_type::eof()) {
			spe::Log::LogString("[ERROR] Prefab file is empty!");
			return nullptr;
		}

		spe::Sprite* current_sprite = nullptr;

		std::vector<spe::Sprite*> mini_repo;

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
			const std::vector<std::string> properties = spe::Utility::Split(line, PREFAB_DELIMITER);
			
			// S stands for sprite
			if (properties[0] == "S")
			{
				spe::Sprite* child = spe::Initializer::InitSprite(properties[1], repo);
				current_sprite = child;
				mini_repo.push_back(child);
			}

			// A stands for animation
			if (properties[0] == "A")
			{
				spe::Initializer::InitAnimation(properties[1], current_sprite);
			}

			// M stands for music (even though its audio component, but A was already taken)
			if (properties[0] == "M") {
				spe::Initializer::InitAudio(properties[1], current_sprite);
			}

			// P stands for particles
			if (properties[0] == "P")
			{
				spe::Initializer::InitParticles(properties[1], current_sprite);
			}
		}

		// Parent algorithm to set childs
		for (int i = 0; i < mini_repo.size(); i++)
		{
			spe::Sprite* const sprite = mini_repo[i];
			if (sprite->GetParentId() > 0)
			{
				spe::Sprite* parent = spe::SpriteRepository::GetWithId(mini_repo, sprite->GetParentId());
				if (parent != nullptr)
				{
					sprite->ptr_Parent = parent;
					parent->ptr_Childs.push_back(sprite);
				}
			}
		}

		spe::Sprite* node = mini_repo[0]->GetNode();
		return node;
	}
	
	throw std::runtime_error("Couldn't open prefab file :(");
	return nullptr;
}

void spe::Initializer::InitTags(spe::SpriteRepository& repo, const std::string& path)
{
	std::fstream tag_file;
	OPEN_FILE(tag_file, path, std::ios::in);  // pass flags here

	if (tag_file.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(tag_file, line))
		{
			cnt++;
			if (cnt == 1)
			{
				continue;
			}
			repo.Tags.push_back(line);
		}
		tag_file.close();
		return;
	}
	throw std::runtime_error("No tags file");
}

void spe::Initializer::InitScenes(SceneHandler& handler, const std::string& path)
{
	std::fstream scene_file;
	OPEN_FILE(scene_file, path, std::ios::in);  // pass flags here


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
			const std::string new_path = std::string("Engine") + PATH_SYMBOL + "Saves" + PATH_SYMBOL + line;
			if (std::filesystem::exists(new_path))
			{
				handler.TotalScenes.push_back(line);
			}
		}

		scene_file.close();
		return;
	}
	throw std::runtime_error("No scene file");
}

void spe::Initializer::IntiHighestSpriteID(spe::SpriteRepository& repo, const std::string& path)
{
	std::fstream indexFile;
	int index = 0;

	OPEN_FILE(indexFile, path, std::ios::in);  // pass flags here


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

	repo.SetHighestId(index);
}
