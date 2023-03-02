#include "UIAssetTools.h"

// Constructor

s2d::UIAssetTools::UIAssetTools()
{
}

// Public functions

void s2d::UIAssetTools::update(bool& hovered)
{
	if (ImGui::IsMouseReleased(1) && hovered)
	{
		ImGui::OpenPopup(this->m_POPUP_NAME.c_str());
	}

	if (ImGui::IsPopupOpen(this->m_POPUP_NAME.c_str()))
	{
		hovered = true;
		ImGui::BeginPopup(this->m_POPUP_NAME.c_str());
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("C++ Script"))
			{
				//  Create a scirpt, link it into the user project
			
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}
}
