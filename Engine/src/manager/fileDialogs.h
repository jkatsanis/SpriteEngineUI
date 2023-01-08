#pragma once

#include <ImGui.h>
#include <dirent.h>
#include <iostream>
#include <manager/fontManager.h>
#include <icons.h>

namespace s2d
{
	enum class CurrentFileDialog
	{
		None = -1,
		Open = 0,
		Create
	};
	class FileDialog
	{
	private:
		bool checkIfADirHasSubDirs(const std::string& dir);

	public:
		std::string pathClicked;

		FileDialog();

		void openFile(const char* dir_path, const std::string& icon);

	public:
		static std::string getEmptyStringBetween(const std::string& content, const std::string& name, float padding);

	};
}


