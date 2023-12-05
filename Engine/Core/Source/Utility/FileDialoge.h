#pragma once

#include <ImGui.h>
#include <dirent.h>
#include <iostream>
#include <Utility/Style.h>
#include <icons.h>
#include <Include/UtilityFunctions.h>
#include <filesystem>

namespace spe
{
	enum class CurrentFileDialog
	{
		None = -1,
		Open = 0,
		Create
	};

	/// <summary>
	/// File dialogs coll the "disableWindow();" function when the "x" button is pressed
	/// Absulute and relative paths will work
	/// </summary>
	class FileDialog
	{
	private:
		bool m_CloseWindow;
		bool m_DisplayTitle;
		std::string m_Icon;
		std::string m_Path;
		std::string m_FirstNodeText;
		ImVec2 m_WindowSize;
		std::string m_Title;
		bool m_IsOpen;
		float m_FontScale;
		bool m_ShowFiles;

		/// <summary>
		/// Opens a file and displays the folder recursivly with <ImGui::TreeNodes>, needs to be called by <displayNoded>
		/// </summary>
		void OpenFile(const char* dir_path);

		bool DisplaySymbol(const std::string& icon, float windowSizeX);
	public:
		std::string PathClicked;
		std::string ItemClicked;
		bool WindowFocus;

		FileDialog();

		/// <summary>
		/// Creats the file dialoge. Only folders
		/// </summary>
		/// <param name="path">The path where the file dialoge while start. For exmaple C:\\ DO USE THE \\!! Absulute and relative paths will work</param>
		/// <param name="icon">The icon next to the FOLDER</param>
		/// <param name="title">The title in the top left corner</param>
		/// <param name="windowSize">The window size of the file dialoge window</param>
		FileDialog(std::string path, std::string icon, std::string title, ImVec2 windowSize, bool show_files, float font_scale);

		/// <summary>
		/// Resets the data of the File Dialog
		/// </summary>
		void DisableWindow();

		/// <summary>
		///  Displays all the nodes including the path you provide
		/// </summary>
		void DisplayNodes();

		/// <summary>
		/// Closes the window, so it basicly resets all fields
		/// </summary>
		/// <returns></returns>
		bool IsWindowClosed() const { return this->m_CloseWindow; }

		/// <summary>
		/// Enables the window, cant be drawn without this function
		/// </summary>
		void EnableWindow();

		/// <summary>
		/// Enables the window, cant be drawn without this function
		/// </summary>
		void EnableWindow(const std::string& title);

		/// <summary>
		/// Sets the first node of the file dialoge, ONLY CHANGES THE LOOK!!
		/// </summary>
		void SetFirstNode(const std::string& top) { this->m_FirstNodeText = top; }

		void Update();

		bool IsItemSelected();

	public:
		static std::string GetEmptyStringBetween(const std::string& content, const std::string& name, float padding);
		static bool CheckIfADirHasSubItems(const std::string& dir, bool show_files);
	};
}


