#pragma once

#include <physicalComponents/sprite.h>
#include <fstream>
#include <vector>
#include <calculationComponents/vector3.h>
#include <physicalComponents/camera.h>
#include <data/engineData.h>	

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

#include <dirent.h>

namespace s2d
{
	namespace flc
	{
		void createSaveFile(std::vector<s2d::Sprite*>& sprite);
		void createWindowBackgroundSaveFile(const s2d::Vector3 windowBackground);
		void createCameraSaveFile(const s2d::Camera& camera);
		void createIndexSaveFile();
		void createKnownProjectDirFile();
		void createAnimtionSaveFile();

		bool isProjectPathValid(const std::string& path);
		bool checkIfProjectExistInFile(std::string& ref);

		/// <summary>
		/// Checks if the absulute path exists in the 'knownProject' file
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		bool checkIfProjcetAlreadyExists(const std::string& path);

		/// <summary>
		/// This method iterates over the 'createKnownProjectDirFile' and removes the
		/// projects which it cant find
		/// </summary>
		void removeInvalidPathsFromFile();
			
		/// <summary>
		/// Copies the CONTENT of the input DIR into the the ouput dir, 
		/// there a folder gets created with the name you provided 
		/// </summary>
		/// <param name="name">The name the folder should have</param>
		/// <returns>A absulute path to the directory</returns>
		std::string copyDir(const std::string& inputDir, const std::string& outputdir, const std::string& name);

		std::string getPropertyLineWithSeperator(Sprite* sprite);
	}
}


