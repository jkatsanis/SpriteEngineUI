#include "FileData.h"

void s2d::FileData::setWindowBackground()
{
	Vector3 vec;
	std::fstream backgroundFile;

	//opening the file where all sprite data is
	backgroundFile.open("engine\\saves\\background.txt", std::ios::in);
	if (backgroundFile.is_open())
	{
		std::cout << "hi";
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

		s2d::FileData::windowBackground = vec;
	}
}

s2d::Vector3 s2d::FileData::windowBackground = s2d::Vector3(0, 0, 0);
