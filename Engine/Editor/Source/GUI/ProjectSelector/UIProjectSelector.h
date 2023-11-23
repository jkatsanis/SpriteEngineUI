#pragma once
#include <Include/dirent.h>
#include <imgui_internal.h>

#include "GUI/Color.h"
#include "GUI/UIModels.h"
#include "Core/GameWindow.h"
#include "Utility/Style.h"
#include "Utility/FileDialoge.h"
#include "GUI/UIModels.h"
#include "Core/Log.h"
#include "Savesystem.h"

#define SELECTOR_WINDOW_SIZE ImVec2(960, 540)
#define SELECTOR_FILE_DIALOG_SIZE ImVec2(700, 400)
#define SELECTOR_FONT_SCALE spe::Style::s_DefaultFontSize + 0.2f

#define DATA_PADDING_TOP 10.0f

namespace spe
{
	class UIProjectSelector
	{
	private:
		spe::GameWindow m_Window;
		std::vector<spe::UserProjectInfo> m_Projects;
		spe::FileDialog m_CreateFileDialoge;
		spe::FileDialog m_OpenFileDialog;
		float m_FontScale;
		bool m_IsOpen;
		std::string m_CreateProjectName;
		std::string m_FullProjectPath;

		std::vector<spe::UserProjectInfo>ReadProjectData();
		void RenderProjectData();
		void RrenderInfoOverProjects(float pad);

		void RenderWindow();

		bool IsProjectPathValid(const std::string& path);
		void RenderFileDialogAndButtons();
		void CreatePopupToCreateProject();
		bool CheckIfprojectExists(const std::string& path);
		void CreateProject();
		void CreateProjectFiles(const std::string& path, const std::string& name);

		void AddProject(const std::string& full, const std::string& name);
		bool TryToOpenProject();

	public:

		UIProjectSelector();

		bool IsOpen() const { return this->m_IsOpen; }
		void Update();
		void Shutdown();
	};
}