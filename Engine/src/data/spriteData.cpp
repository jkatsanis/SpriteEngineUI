#include "spriteData.h"

int s2d::SpriteData::getHighestIndex()
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

	return index;
}

std::string s2d::SpriteData::defaultSpritePath = "EngineAssets\\Sprites\\default.png";

/// <summary>
/// This is the highest sprite id currently. 
/// NO SPRITE is allowed to have the same ID. Always increment when creating new sprite
/// </summary>
int s2d::SpriteData::highestSpriteID = -1;
int s2d::SpriteData::dupeNameCounter = 0;

