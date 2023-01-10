#pragma once

#include <ImGui.h>
#include <iostream>
#include <imgui_internal.h>
#include <manager/fileDialogs.h>
#include <data/projectInfo.h>
#include <projectSelectorColors.h>
#include <manager/saveSystem.h>

namespace s2d
{
	class UIProjectSelectorProjectSection
	{
	private:
		ImVec2 m_createWindowSize;

		s2d::CurrentFileDialog m_currentFileDialoge;
		s2d::FileDialog m_createFileDialoge;

		const s2d::UIProjectSelectorLocation* m_ptr_UILocation;

		std::vector<s2d::ProjectInfo> m_projects;

		void renderInfoOverProjects(float padding);
		void renderProjectData();
		void renderFileDialogs();
		void createPopupToCreateProject();

		void createProject();

		std::string name;

		std::vector<s2d::ProjectInfo> readProjectInfosFromFile();
		s2d::CurrentFileDialog getFileDialogeWhichUserWantsToMake();

		void init();

	public:
		UIProjectSelectorProjectSection();
		UIProjectSelectorProjectSection(const s2d::UIProjectSelectorLocation* location);

		void update();
	};
}

