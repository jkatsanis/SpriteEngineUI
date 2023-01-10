#include "engineData.h"


std::string s2d::EngineData::initPathToEngine()
{
	std::string path;
	std::fstream engineToProjectFile;

	//opening the file where all sprite data is
	engineToProjectFile.open("saves\\gameWindow.txt", std::ios::in);
	if (engineToProjectFile.is_open())
	{
		std::string line;
		int cnt = 0;
		while (std::getline(engineToProjectFile, line))
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

			//INITIIALIZING PROPS
			path = propertys[0];
		}
		engineToProjectFile.close();

		return path;
	}

	return " ";
}

/// <summary>
/// IMPORTANT! The default path goes always from the engine 
/// </summary>
std::string s2d::EngineData::s_pathFromEngineToUserProject = s2d::EngineData::initPathToEngine();

