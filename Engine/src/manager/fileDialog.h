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
	/// <summary>
	/// File dialogs coll the "disableWindow();" function when the "x" button is pressed
	/// Absulute and relative paths will work
	/// </summary>
	class FileDialog
	{
	private:
		bool m_closeWindow;
		bool m_displayTitle;
		std::string m_icon;
		std::string m_path;
		std::string m_firstNodeText;
		ImVec2 m_windowSize;
		std::string m_title;
		bool is_open;

		/// <summary>
		/// Opens a file and displays the folder recursivly with <ImGui::TreeNodes>, needs to be called by <displayNoded>
		/// </summary>
		void openFile(const char* dir_path);

		bool displaySymbol(const std::string& icon, float windowSizeX);
	public:
		std::string pathClicked;
		std::string itemClicked;
		bool windowFocus;

		FileDialog();

		/// <summary>
		/// Creats the file dialoge. Only folders
		/// </summary>
		/// <param name="path">The path where the file dialoge while start. For exmaple C:\\ DO USE THE \\!! Absulute and relative paths will work</param>
		/// <param name="icon">The icon next to the FOLDER</param>
		/// <param name="title">The title in the top left corner</param>
		/// <param name="windowSize">The window size of the file dialoge window</param>
		FileDialog(std::string path, std::string icon, std::string title, ImVec2 windowSize);

		/// <summary>
		/// Rests the data of the File Dialog
		/// </summary>
		void disableWindow();

		/// <summary>
		///  Displays all the nodes including the path you provide
		/// </summary>
		void displayNodes();

		/// <summary>
		/// Closes the window, so it basicly resets all fields
		/// </summary>
		/// <returns></returns>
		bool isWindowClosed() const { return this->m_closeWindow; }

		/// <summary>
		/// Enables the window, cant be drawn without this function
		/// </summary>
		void enableWindow();

		/// <summary>
		/// Enables the window, cant be drawn without this function
		/// </summary>
		void enableWindow(const std::string& title);

		/// <summary>
		/// Sets the first node of the file dialoge, ONLY CHANGES THE LOOK!!
		/// </summary>
		void setFirstNode(const std::string& top) { this->m_firstNodeText = top; }

		void update();

		bool IsItemSelected();

	public:
		static std::string getEmptyStringBetween(const std::string& content, const std::string& name, float padding);
		static bool checkIfADirHasSubDirs(const std::string& dir);
	};
}


