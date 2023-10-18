#pragma once

#include <ImGui.h>
#include <iostream>
#include <imgui_internal.h>
#include <manager/fileDialog.h>
#include <data/projectInfo.h>
#include <fstream>
#include <data/engineData.h>
#include <manager/saveSystem.h>
#include <algorithm>
#include <guiComponents/colors.h>

namespace s2d
{
	class UIProjectSelectorProjectSection
	{
	private:
		std::string m_createProjectName;

		ImVec2 m_createWindowSize;
		s2d::CurrentFileDialog m_currentFileDialoge;

		s2d::FileDialog m_createFileDialoge;
		s2d::FileDialog m_openFileDialog;

		const s2d::UIProjectSelectorLocation* m_ptr_UILocation;

		std::vector<s2d::UserProjectInfo> m_projects;

		void renderInfoOverProjects(float padding);
		void renderProjectData();
		void renderFileDialogs();
		void createPopupToCreateProject();
		void tryToOpenProject();

		std::vector<s2d::UserProjectInfo> readProjectInfosFromFile();
		s2d::CurrentFileDialog getFileDialogeWhichUserWantsToMake();

		void init();
		void createProject();

	public:
		UIProjectSelectorProjectSection();
		UIProjectSelectorProjectSection(const s2d::UIProjectSelectorLocation* location);

		void update();
	};
}

