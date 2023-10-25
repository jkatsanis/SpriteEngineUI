#include "saveSystem.h"
#include <string>

void s2d::flc::saveEverything(const s2d::Vector3& bg, s2d::SpriteRepository& repo, s2d::GUIRepository& gui_repo, std::vector<std::string>& scenes)
{
	//Stopping all animations so there wont be saved the current path of the sprite
	s2d::Animator::stopAllAnimations(repo);

	//We need to save our data | Dont forget to save it also in "UIToolButtons"
	s2d::flc::createSceneSaveFile(scenes);
	s2d::flc::createSaveFile(repo);
	s2d::flc::createWindowBackgroundSaveFile(bg);
	s2d::flc::createCameraSaveFile(gui_repo.camera);
	s2d::flc::createIndexSaveFile(repo);
	s2d::flc::createKnownAnimationFile(repo);
	s2d::flc::createAnimtionSaveFiles(repo);
	s2d::flc::createTagSaveFile(repo);
	// Known projects file gets created in project selector
}

void s2d::flc::createTagSaveFile(s2d::SpriteRepository& repo)
{
	std::fstream tag_file;
	tag_file.open(PATHT_TO_TAG_FILE, std::ios::out);
	const std::vector<std::string>& tags = repo.getTags();

	if (tag_file.is_open())
	{	
		tag_file << "Tags" << "\n";

		for (int i = 0; i < tags.size(); i++)
		{
			tag_file << tags[i] << "\n";
		}

		tag_file.close();
	}
}

void s2d::flc::createSaveFile(const s2d::SpriteRepository& spriteRepo)
{
	std::fstream spriteFile;
	spriteFile.open(PATH_TO_SPRITE_FILE, std::ios::out);

	if (spriteFile.is_open()) 
	{
		spriteFile << "name;vecpos;transformPosX;transformPosY;ScaleX;ScaleY;rotation;filepath;boxColliderWidthLeftOrRightX;boxColliderWidthLeftOrRighY;boxColliderHeightUpOrDownX;boxColliderHeightUpOrDownY;boxColliderExists;solid;sortingLayer;gravity;mass;physicsBodyExists;id;parentId;nextPosX;nextPosY;lastPosX;lastPosY;listPos;highestChild;positionToParentX;positionToParentY;animatorExists;prefabExist;loadInMemory;pathToPrefab" << "\n";
		for (int i = 0; i < spriteRepo.amount(); i++)
		{
			const s2d::Sprite* const sprite = spriteRepo.readAt(i, true);

			std::string line = getPropertyLineWithSeperator(sprite);

			spriteFile << line << "\n";
		}

		spriteFile.close();
	}
}

std::string s2d::flc::getPropertyLineWithSeperator(const Sprite* const sprite)
{
	std::string line;
	const std::string transformPosX = std::to_string(sprite->transform.position.x);
	const std::string transformPosY = std::to_string(sprite->transform.position.y);
	const std::string scaleX = std::to_string(sprite->transform.getScale().x);
	const std::string scaleY = std::to_string(sprite->transform.getScale().y);
	const std::string spritePath = std::splitStringTillLastWord(sprite->sprite_renderer.path, s2d::EngineData::s_path_to_user_project);

	const std::string boxColliderWidthLeftOrRightX = std::to_string(sprite->collider.box_collider_width.x);
	const std::string boxColliderWidthLeftOrRightY = std::to_string(sprite->collider.box_collider_width.y);

	const std::string boxColliderHeightUpOrDownX = std::to_string(sprite->collider.box_collider_height.x);
	const std::string boxColliderHeightUpOrDownY = std::to_string(sprite->collider.box_collider_height.y);

	const std::string colliderExists = std::boolToStr(sprite->collider.exist);
	const std::string isSolid = std::boolToStr(sprite->collider.is_solid);
	const std::string sortingLayer = std::to_string(sprite->sprite_renderer.sorting_layer_index);
	const std::string gravity = std::to_string(sprite->physicsBody.gravity);
	const std::string mass = std::to_string(sprite->physicsBody.mass);
	const std::string bodyExist = std::boolToStr(sprite->physicsBody.exist);
	const std::string id = std::to_string(sprite->getId());
	const std::string parentId = std::to_string(sprite->getParentId());

	const std::string nextPosX = std::to_string(sprite->transform.next_pos.x);
	const std::string nextPosY = std::to_string(sprite->transform.next_pos.y);

	const std::string lastPosX = std::to_string(sprite->transform.last_pos.x);
	const std::string lastPosY = std::to_string(sprite->transform.last_pos.y);

	const std::string positionToParentX = std::to_string(sprite->transform.position_to_parent.x);
	const std::string positionToParentY = std::to_string(sprite->transform.position_to_parent.y);

	const std::string animatorExist = std::boolToStr(sprite->animator.exist);

	const std::string prefabExist = std::boolToStr(sprite->prefab.exist);
	const std::string loadInMemory = std::boolToStr(sprite->prefab.load_in_memory);
	const std::string pathToPrefab = sprite->prefab.engine_path_to_file;
	const std::string rotation = std::to_string(sprite->transform.getRotation());
	const std::string tag = sprite->tag;

	//Name, vec, transform path, rotation
	line = sprite->name + ";" + "0" + ";" + transformPosX + ";" + transformPosY + ";" + scaleX + ";" + scaleY + ";" + spritePath + ";" + rotation;

	//BoxCollider
	line += ";" + boxColliderWidthLeftOrRightX + ";" + boxColliderWidthLeftOrRightY + ";" + boxColliderHeightUpOrDownX + ";" + boxColliderHeightUpOrDownY + ";" + colliderExists + ";" + isSolid;

	//Sorting layer
	line += ";" + sortingLayer;
	
	//PhysicsBody
	line += ";" + gravity + ";" + mass + ";" + bodyExist;

	//ParentId, id
	line += ";" + id + ";" + parentId;

	//next pos, last pos
	line += ";" + nextPosX + ";" + nextPosY + ";" + lastPosX + ";" + lastPosY;

	//list pos ( childing)
	line += ";" + std::string("listpos") + ";" + "childCount";

	//Pos to parent (x, y)
	line += ";" + positionToParentX + ";" + positionToParentY;

	//ANIMATIONS

	line += ";" + animatorExist;

	// Prefab

	line += ";" + prefabExist + ";" + loadInMemory + ";" + pathToPrefab;

	// General
	line += ";" + tag;
 	
	return line;
}

void s2d::flc::createWindowBackgroundSaveFile(const s2d::Vector3 windowBackground)
{
	std::fstream backgroundFile;

	backgroundFile.open(PATH_TO_BACKGROUND_FILE, std::ios::out);

	if (backgroundFile.is_open())
	{
		backgroundFile << "Red;Blue;Green" << "\n";
	
		std::string line = std::to_string(windowBackground.x) + ";" + std::to_string(windowBackground.y) + ";" + std::to_string(windowBackground.z);

		backgroundFile << line << "\n";

		backgroundFile.close();
	}

}

void s2d::flc::createCameraSaveFile(const s2d::Camera& camera)
{
	std::fstream backgroundFile;

	backgroundFile.open(PATH_TO_CAMERA_FILE, std::ios::out);

	if (backgroundFile.is_open())
	{
		backgroundFile << "TransformPoxX;TransformPosY;Zoom;Speed" << "\n";

		std::string line = std::to_string(camera.transform.position.x) + ";" +
			std::to_string(camera.transform.position.y) + ";" +
			std::to_string(camera.camera_zoom) + ";" + 
			std::to_string(camera.camera_speed);

		backgroundFile << line << "\n";

		backgroundFile.close();
	}
}

void s2d::flc::createIndexSaveFile(s2d::SpriteRepository& repo)
{
	int index = repo.highest_sprite_id;
	std::fstream indexFile;

	indexFile.open(PATH_TO_INDEX_FILE, std::ios::out);
	if (indexFile.is_open())
	{
		indexFile << "highestIndex" << "\n";

		indexFile << index << "\n";

		indexFile.close();
	}
}

void s2d::flc::createKnownProjectDirFile()
{
	std::string content;
	if (s2d::flc::checkIfProjectExistInFile(content))
	{
		return;
	}

	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	struct tm  timeinfo;
	char buffer[80];
	gmtime_s(&timeinfo, &in_time_t);

	std::strftime(buffer, sizeof(buffer), "%Y/%d/%m %X", &timeinfo);

	const char* relative_path = s2d::EngineData::s_path_to_user_project.c_str();
	char absolute_path[FILENAME_MAX];
	if (!_fullpath(absolute_path, relative_path, FILENAME_MAX) != NULL) 
	{
		std::cerr << "Error getting absolute path" << std::endl;
	}

	std::string date(buffer);
	std::string name = s2d::EngineData::s_name_of_user_project;
	std::string absulutePathStr = absolute_path;

	std::string path = name + ";" + absulutePathStr + ";" + date + ";" + relative_path;

	std::ofstream knownProjectFile;

	knownProjectFile.open(PATH_TO_KNOWN_PROJECTS, std::ios::out);
	if (knownProjectFile.is_open())	
	{
		knownProjectFile << "name;absulutePathStr;date;relativePath" << "\n";

		knownProjectFile << content + path << "\n";

		knownProjectFile.close();
	}


}

void s2d::flc::createAnimtionSaveFiles(const s2d::SpriteRepository& spriteRepository)
{
	for (int i = 0; i < spriteRepository.amount(); i++)
	{
		const s2d::Sprite* const ptr_sprite = spriteRepository.readAt(i, true);

		if (ptr_sprite->animator.exist)
		{
			for (const auto& anim : ptr_sprite->animator.animations)
			{
				createAnimationSaveFile(ptr_sprite, anim.second);
			}
		}
	}
}

void s2d::flc::createAnimationSaveFile(const s2d::Sprite* ptr_sprite, const s2d::Animation& animationToSave)
{
	std::string name = animationToSave.getName();
	std::string content =
		animationToSave.getName() +"\n" +
		std::to_string(ptr_sprite->getId()) + "\n" +
		std::boolToStr(animationToSave.loop) + "\n";

	const std::vector<s2d::KeyFrame>& frames = animationToSave.getKeyFrames();

	for (const s2d::KeyFrame& frame : frames)
	{
		content += std::to_string(frame.delay) + std::string(";") + 
			s2d::UI::getUserProjectPathSeperatetFromEnginePath(frame.path) + "\n";
	}

	std::string pathAndName = s2d::EngineData::s_path_to_user_project + "\\" + animationToSave.getUserPathToFile();
	std::createFileWithContent(content, pathAndName);
}

void s2d::flc::createSceneSaveFile(const std::vector<std::string>& scene_names)
{
	std::string content = "Scenes\n";

	for (size_t i = 0; i < scene_names.size(); i++)
	{
		content += scene_names[i] + "\n";
	}
	const std::string path = PATH_TO_SCENE_FILE;
	std::createFileWithContent(content, path);
}

void s2d::flc::createOrUpdatePrefabFile(const s2d::Sprite* content, const std::string& pathToFile, const std::string& oldFilePath)
{
	//Getting filelocation as: \\assets
	std::string fileContent = "";

	fileContent += "name;vecpos;transformPosX;transformPosY;ScaleX;ScaleY;filepath;boxColliderWidthLeftOrRightX;boxColliderWidthLeftOrRighY;boxColliderHeightUpOrDownX;boxColliderHeightUpOrDownY;boxColliderExists;solid;sortingLayer;gravity;mass;physicsBodyExists;id;parentId;nextPosX;nextPosY;lastPosX;lastPosY;listPos;highestChild;positionToParentX;positionToParentY;animatorExists;prefabExist;loadInMemory;pathToPrefab\n";

	fileContent += getPropertyLineWithSeperator(content) + "\n";

	if (pathToFile != oldFilePath && oldFilePath != "")
	{
		std::removeFile(oldFilePath);
	}

	std::vector<const s2d::Sprite*> childs;

	s2d::SpriteRepository::getAllChilds(childs, content);

	for (size_t i = 0; i < childs.size(); i++)
	{
		fileContent += getPropertyLineWithSeperator(childs[i]) + "\n";
	}
	if (content->animator.exist)
	{
		for (auto& animation : content->animator.animations)
		{
			const auto& value = animation.second;
			fileContent += value.getUserPathToFile() + "\n";
		}
	}

	std::createFileWithContent(fileContent, pathToFile);
}

void s2d::flc::createKnownAnimationFile(const s2d::SpriteRepository& spriteRepository)
{
	std::string content = "PathToAnimation\n";

	for (int i = 0; i < spriteRepository.amount(); i++)
	{
		const s2d::Sprite* const sprite = spriteRepository.readAt(i, true);

		for (const auto& animation : sprite->animator.animations)
		{
			/// NO SEPERATION SINCE IT GETS ON "CREATE ANIMATION" SEPERATED
			content  += animation.second.getUserPathToFile() + "\n";
		}
	}

	std::createFileWithContent(content, PATH_TO_KNOWN_ANIMATIONS);
}

bool s2d::flc::isProjectPathValid(const std::string& path)
{
	// TODO logic to chck if verify.vsn is valid, just simple now
	std::ifstream file(path);
	bool good = file.good();
	file.close();
	return good;
}

bool s2d::flc::checkIfProjectExistInFile(std::string& ref)
{
	const int INDEX_AT_PATH = 1;

	bool found = false;
	std::string searchPath = s2d::EngineData::s_path_to_user_project;
	std::fstream knownProjectFile;

	char absulutPath[1024];

	if(!_fullpath(absulutPath, searchPath.c_str(), FILENAME_MAX) != NULL)
	{
		std::cerr << "Error getting absolute path" << std::endl;
	}

	//opening the file where all sprite data is
	knownProjectFile.open(PATH_TO_KNOWN_PROJECTS, std::ios::in);
	if (knownProjectFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(knownProjectFile, line))
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

			ref += line + "\n";
			
			found = propertys[INDEX_AT_PATH] == absulutPath;

			if (found)
			{
				knownProjectFile.close();
				return true;
			}
		}
		knownProjectFile.close();
	}

	return found;
}

bool s2d::flc::checkIfProjcetAlreadyExists(const std::string& path)
{
	const int INDEX_AT_PATH = 1;

	bool found = false;
	std::fstream knownProjectFile;

	//opening the file where all sprite data is
	knownProjectFile.open(PATH_TO_KNOWN_PROJECTS, std::ios::in);
	if (knownProjectFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(knownProjectFile, line))
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

			found = propertys[INDEX_AT_PATH] == path;

			if (!found)
			{
				found = propertys[INDEX_AT_PATH] + "\\" == path;
			}

			if (found)
			{
				knownProjectFile.close();
				return found;
			}
		}
		knownProjectFile.close();
	}

	return found;
}

void s2d::flc::removeInvalidPathsFromFile()
{
	std::vector<std::string> validlines;

	std::fstream knownProjectFile;
	//opening the file where all sprite data is
	knownProjectFile.open(PATH_TO_KNOWN_PROJECTS, std::ios::in);
	if (knownProjectFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(knownProjectFile, line))
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
			
			std::string absulutePath = propertys[1];

			DIR* dir = opendir(absulutePath.c_str());
			if (dir)
			{
				validlines.push_back(line);
			}

			closedir(dir);
		}
		knownProjectFile.close();
	}

	std::fstream knownProject;

	knownProject.open(PATH_TO_KNOWN_PROJECTS, std::ios::out);
	if (knownProject.is_open())
	{
		knownProject << "name;absulutePathStr;date;relativePath" << "\n";

		for (const std::string& line : validlines)
		{
			knownProject << line << "\n";
		}

		knownProject.close();
	}
}

std::string s2d::flc::copyDir(const std::string& inputDir, const std::string& outputdir, const std::string& name)
{
	std::string mkdir = "mkdir \"" + outputdir + std::string(name.c_str()) + "\"";

	system(mkdir.c_str());

	std::string copy = "xcopy \"" + inputDir + "\" \"" + outputdir + "\\" + std::string(name.c_str()) + "\" /E";

	system(copy.c_str());

	return outputdir + std::string(name.c_str());
}

void s2d::flc::removeDir(const std::string& path)
{
	std::string rmDir = "rmdir " + path + " /s /q";
	system(rmDir.c_str());
}

std::string s2d::flc::copyDir(const std::string& inputDir, const std::string& outputdir, const std::string& name, const std::vector<std::string>& exclude)
{
	struct stat sb;

	if (stat(inputDir.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR)
		|| stat(outputdir.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR))
	{
		std::cout << "LOG [ERROR] Cant copy folder because input or output dir is invalid";
		return "-1";
	}
	std::fstream fs;

	fs.open("exclude_file.txt", std::fstream::out);

	if (fs.is_open())
	{
		for (int i = 0; i < exclude.size(); i++)
		{
			fs << exclude[i] << "\n";
		}
		fs << "\\src\\" << "\n";
		fs << ".cpp" << "\n";
		fs << ".h" << "\n";
	}

	fs.close();

	std::string mkdir = "mkdir " + outputdir + std::string(name.c_str());

	system(mkdir.c_str());

	std::string copy = "xcopy /exclude:exclude_file.txt " + inputDir + " " + outputdir + std::string(name.c_str()) + " /E";

	system(copy.c_str());
	std::string remove_exclusion_file = "del exclude_file.txt";

	system(remove_exclusion_file.c_str());
	return outputdir + std::string(name.c_str());
}

void s2d::flc::cleanUp(s2d::SpriteRepository& repo, bool save_sprites)
{
	// Deleting the prefab file if the sprite has no prfb file. It compares it's name with the prefab names
	std::vector<std::string> valid_prefab_names;
	const std::string path = s2d::EngineData::s_path_to_user_project + "\\assets";
	std::getFilePathWithExtensionInFolder(path, EXTENSION_PREFAB_FILE, valid_prefab_names);

	for (size_t i = 0; i < valid_prefab_names.size(); i++)
	{
		valid_prefab_names[i] = std::getFileOnPath(valid_prefab_names[i]);
	}

	for (size_t i = 0; i < repo.amount(); i++)
	{
		const std::string engine_name = std::getFileOnPath(repo.readAt(i)->prefab.engine_path_to_file);

		// Updating all the prefab files, and checking if they still exist
		if (!std::isEqualWithAny(engine_name, valid_prefab_names))
		{
			repo.readAt(i)->prefab.reset();
		}
	}
	s2d::flc::createKnownAnimationFile(repo);
}

