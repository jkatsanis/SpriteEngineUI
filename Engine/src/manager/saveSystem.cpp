#include "saveSystem.h"
#include <string>

void s2d::flc::createSaveFile(std::vector<s2d::Sprite*>& sprites)
{
	std::fstream spriteFile;

	spriteFile.open(PATH_TO_SPRITE_FILE, std::ios::out);

	if (spriteFile.is_open()) 
	{
		spriteFile << "name;vecpos;transformPosX;transformPosY;filepath;boxColliderWidthLeftOrRightX;boxColliderWidthLeftOrRighY;boxColliderHeightUpOrDownX;boxColliderHeightUpOrDownY;boxColliderExists;solid;sortingLayer;gravity;mass;physicsBodyExists;id;parentId;nextPosX;nextPosY;lastPosX;lastPosY;listPos;highestChild;positionToParentX;positionToParentY;animatorExists" << "\n";
		for (Sprite* spr : sprites)
		{
			std::string line = getPropertyLineWithSeperator(spr);

			spriteFile << line << "\n";
		}

		spriteFile.close();
	}

}

std::string s2d::flc::getPropertyLineWithSeperator(Sprite* sprite)
{
	auto boolToStr = [](const bool& b)
	{
		return b ? "True" : "False";
	};

	std::string line;
	std::string vecpos = std::to_string(sprite->getVectorPosition());
	std::string transformPosX = std::to_string(sprite->transform.position.x);
	std::string transformPosY = std::to_string(sprite->transform.position.y);
	std::string spritePath = std::splitStringTillLastWord(sprite->path, s2d::EngineData::s_pathToUserProject);

	std::string boxColliderWidthLeftOrRightX = std::to_string(sprite->collider.boxColliderWidthLeftOrRight.x);
	std::string boxColliderWidthLeftOrRightY = std::to_string(sprite->collider.boxColliderWidthLeftOrRight.y);

	std::string boxColliderHeightUpOrDownX = std::to_string(sprite->collider.boxColliderHeightUpOrDown.x);
	std::string boxColliderHeightUpOrDownY = std::to_string(sprite->collider.boxColliderHeightUpOrDown.y);

	std::string colliderExists = boolToStr(sprite->collider.exists);
	std::string isSolid = boolToStr(sprite->collider.isSolid);
	std::string sortingLayer = std::to_string(sprite->sortingLayerIndex);
	std::string gravity = std::to_string(sprite->physicsBody.gravity);
	std::string mass = std::to_string(sprite->physicsBody.mass);
	std::string bodyExist = boolToStr(sprite->physicsBody.exists);
	std::string id = std::to_string(sprite->getId());
	std::string parentId = std::to_string(sprite->getParentId());

	std::string nextPosX = std::to_string(sprite->transform.nextPos.x);
	std::string nextPosY = std::to_string(sprite->transform.nextPos.y);

	std::string lastPosX = std::to_string(sprite->transform.lastPos.x);
	std::string lastPosY = std::to_string(sprite->transform.lastPos.y);

	std::string listPos = std::to_string(sprite->getChildListPosition());
	std::string childCount = std::to_string(sprite->getChildCount());

	std::string positionToParentX = std::to_string(sprite->transform.positionToParent.x);
	std::string positionToParentY = std::to_string(sprite->transform.positionToParent.y);

	std::string animatorExist = boolToStr(sprite->animator.exists);

	//Name, vec, transform path
	line = sprite->name + ";" + vecpos + ";" + transformPosX + ";" + transformPosY + ";" + spritePath;

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
	line += ";" + listPos + ";" + childCount;

	//Pos to parent (x, y)
	line += ";" + positionToParentX + ";" + positionToParentY;

	//ANIMATIONS

	line += ";" + animatorExist;
	
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
		backgroundFile << "TransformPoxX;TransformPosY;Zoom" << "\n";

		std::string line = std::to_string(camera.transform.position.x) + ";" + std::to_string(camera.transform.position.y) + ";" + std::to_string(camera.cameraZoom);

		backgroundFile << line << "\n";

		backgroundFile.close();
	}
}

void s2d::flc::createIndexSaveFile()
{
	int index = s2d::SpriteData::highestSpriteID;
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
	errno_t err = gmtime_s(&timeinfo, &in_time_t);

	std::strftime(buffer, sizeof(buffer), "%Y/%d/%m %X", &timeinfo);


	const char* relative_path = s2d::EngineData::s_pathToUserProject.c_str();
	char absolute_path[FILENAME_MAX];
	if (!_fullpath(absolute_path, relative_path, FILENAME_MAX) != NULL) 
	{
		std::cerr << "Error getting absolute path" << std::endl;
	}

	std::string date(buffer);
	std::string name = s2d::EngineData::s_nameOfUserProject;
	std::string absulutePathStr = absolute_path;

	std::string path = name + ";" + absulutePathStr + ";" + date + ";" + relative_path;
	content += path;

	std::ofstream indexFile;

	indexFile.open(PATH_TO_KNOWN_PROJECTS, std::ios::out);
	if (indexFile.is_open())	
	{
		indexFile << "knownProjects" << "\n";

		indexFile << content << "\n";

		indexFile.close();
	}
}

bool s2d::flc::checkIfProjectExistInFile(std::string& ref)
{
	const int INDEX_AT_PATH = 1;

	bool found = false;
	std::string searchPath = s2d::EngineData::s_pathToUserProject;
	std::fstream backgroundFile;

	char absulutPath[1024];

	if(!_fullpath(absulutPath, searchPath.c_str(), FILENAME_MAX) != NULL)
	{
		std::cerr << "Error getting absolute path" << std::endl;
	}

	//opening the file where all sprite data is
	backgroundFile.open(PATH_TO_KNOWN_PROJECTS, std::ios::in);
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
			std::string* propertys = std::splitString(line, delimiter);

			ref += line + "\n";
			
			found = propertys[INDEX_AT_PATH] == absulutPath;

			if (found)
			{
				backgroundFile.close();
				return true;
			}
		}
		backgroundFile.close();
	}
	return found;
}

