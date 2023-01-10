#pragma once

#include <ImGui.h>
#include <dirent.h>
#include <iostream>
#include <manager/fontManager.h>
#include <icons.h>
#include <stdExtension.h>
#include <filesystem>

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
		bool m_closeWindow;
		bool m_displayedTitle;
	    std::string m_ICON;
		std::string m_TITLE;
		std::string m_PATH;

		ImVec2 m_WINDOW_SIZE;

		bool checkIfADirHasSubDirs(const std::string& dir);

		/// <summary>
		/// Opens a file and displays the folder recursivly with <ImGui::TreeNodes>, needs to be called by <displayNoded>
		/// </summary>
		void openFile(const char* dir_path);

		/// <returns>If the icon got pressed</returns>
		bool displaySymbol(const std::string& icon, float windowSizeX);
	public:
		std::string pathClicked;	
		std::string folderClicked;

		FileDialog();
		FileDialog(std::string path, std::string icon, std::string title, ImVec2 windowSize);


		/// <summary>
		///  Displays all the nodes including the path you provide
		/// </summary>
		void displayNodes();
		
		bool closeWindow() const { return this->m_closeWindow; }

	public:
		static std::string getEmptyStringBetween(const std::string& content, const std::string& name, float padding);

	};
}


