#include "spriteData.h"

int s2d::SpriteData::getHighestIndex()
{
	std::fstream indexFile;
	int index = 0;

	//opening the file where all sprite data is
	indexFile.open("engine\\saves\\index.txt", std::ios::in);
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

	return index;
}


/// <summary>
/// This is the highest sprite id currently. 
/// NO SPRITE is allowed to have the same ID. Always increment when creating new sprite
/// </summary>
int s2d::SpriteData::highestSpriteID = s2d::SpriteData::getHighestIndex();
