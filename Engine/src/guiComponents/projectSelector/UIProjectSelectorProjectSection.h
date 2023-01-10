#pragma once

#include <ImGui.h>
#include <iostream>
#include <imgui_internal.h>
#include <manager/fileDialogs.h>
#include <data/projectInfo.h>

namespace s2d
{
	class UIProjectSelectorProjectSection
	{
	private:
		ImVec2 m_createWindowSize;
		ImVec4 m_createButtonsColor;
		ImVec4 m_listBoxHeaderBackground;

		s2d::CurrentFileDialog m_currentFileDialoge;
		s2d::FileDialog m_createFileDialoge;

		const s2d::UIProjectSelectorLocation* m_ptr_UILocation;

		std::vector<s2d::UserProjectInfo> m_projects;

		void renderInfoOverProjects(float padding);
		void renderProjectData();
		void renderFileDialogs();
		void createPopupToCreateProject();

		std::vector<s2d::UserProjectInfo> readProjectInfosFromFile();
		s2d::CurrentFileDialog getFileDialogeWhichUserWantsToMake();

		void init();

	public:
		UIProjectSelectorProjectSection();
		UIProjectSelectorProjectSection(const s2d::UIProjectSelectorLocation* location);

		void update();
	};
}

