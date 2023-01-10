#include "spriteData.h"

int s2d::SpriteData::getHighestIndex()
{
	std::fstream indexFile;
	int index = 0;

	std::string pathToIndexFile = s2d::ProjectInfo::s_pathToUserProject + "\\saves\\index.txt";

	//opening the file where all sprite data is
	indexFile.open(pathToIndexFile, std::ios::in);
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

	return index;
}

std::string s2d::SpriteData::defaultSpritePath = "EngineAssets/Sprites/default.png";
int s2d::SpriteData::highestIndex = -1;
int s2d::SpriteData::dupeNameCounter = 0;

