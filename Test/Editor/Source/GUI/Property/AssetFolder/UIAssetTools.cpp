#include "UIAssetTools.h"

// Constructor

spe::UIAssetTools::UIAssetTools()
{
	this->m_is_popup_open = false;
	this->m_window_font_size = 1;
	this->m_ptr_hovered_icon_name = nullptr;
	this->m_ptr_currentAssetPath = nullptr;
	this->m_classFileName[0] = '\0';
	this->m_open_file_input = false;
}

spe::UIAssetTools::UIAssetTools(const std::string* currentAssetPath, std::string* hoveredIconName)
{
	this->m_is_popup_open = false;
	this->m_classFileName[0] = '\0';
	this->m_open_file_input = false;
	this->m_ptr_currentAssetPath = currentAssetPath;
	this->m_ptr_hovered_icon_name = hoveredIconName;
	this->m_window_font_size = 1;
}

// Public functions

void spe::UIAssetTools::update()
{
	this->getFileName();

	if (this->m_classFileName[0] != '\0' && !this->m_open_file_input)
	{
		this->createFileContent();
		this->m_classFileName[0] = '\0';
	}
	ImGui::SetWindowFontScale(this->m_window_font_size);

	if (ImGui::IsMouseReleased(1))
	{
		ImGui::OpenPopup(INPUT_POPUP_NAME);
	}

	if (ImGui::IsPopupOpen(INPUT_POPUP_NAME))
	{
		this->m_is_popup_open = true;
		ImGui::BeginPopup(INPUT_POPUP_NAME);
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("C++ Script"))
			{
				//  Create a scirpt, link it into the user project
				this->m_open_file_input = true;
			}
			ImGui::EndMenu();
		}
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7);
		if (ImGui::Button("Delete"))
		{
			const std::string d = *this->m_ptr_currentAssetPath;
		    std::string m = *this->m_ptr_hovered_icon_name;
			
			if (m != "")
			{
				std::string file_name = "";

				for (int i = 2; i < m.size(); i++)
				{
					file_name.push_back(m[i]);
				}

				const std::string delete_path = d + "\\" + file_name;

				spe::Utility::Delete(delete_path);

				*this->m_ptr_hovered_icon_name = "";
			}
		}
		ImGui::EndPopup();
	}
	else
	{
		this->m_is_popup_open = false;
	}
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
}

void spe::UIAssetTools::getFileName()
{
	if (!this->m_open_file_input)
	{
		return;
	}

	if (ImGui::Begin("##input_file_name", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::SetNextItemWidth(290);
		ImGui::InputTextWithHint("##file_input", "<name>", this->m_classFileName, CHAR_MAX);
		if (ImGui::Button("Create"))
		{
			this->m_open_file_input = false;
		}

		spe::UIUtility::SetWindowScreenMiddle(ImVec2(300, 100));
		ImGui::End();
	}
}

void spe::UIAssetTools::createFileContent()
{
	std::string header_content =
		"#pragma once\n\n"
		"#include <_header/SpriteEngine.h>\n\n"
		"class " + std::string(this->m_classFileName) + " : public spe::Base\n"
		"{\n"
		"public:\n"
		"   void update();\n"
		"   void start();\n"
		"};\n";

	std::string cpp_content =
		"#include \"" + std::string(this->m_classFileName) + ".h\"\n\n"
		"void " + std::string(this->m_classFileName) + "::start()\n"
		"{\n"
		"\n"
		"}\n"
		"\n"
		"void " + std::string(this->m_classFileName) + "::update()\n"
		"{\n"
		"\n"
		"}\n"
		"\n";

	std::string path = *this->m_ptr_currentAssetPath + "\\" + this->m_classFileName;

	std::string cpp_name = std::string(this->m_classFileName);
	std::string header_name = std::string(this->m_classFileName);

	std::string cpp_file_path = *this->m_ptr_currentAssetPath + "\\" + header_name + ".cpp";
	std::string hpp_file_path = *this->m_ptr_currentAssetPath + "\\" + header_name + ".h";

	spe::Utility::CreateFileWithContent(header_content, hpp_file_path);
	spe::Utility::CreateFileWithContent(cpp_content, cpp_file_path);

	// Building the project of the user when he inserts a new thingy
	spe::EngineData::BuildProject();
}
