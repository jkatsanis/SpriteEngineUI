#include "UIAssetTools.h"

// Constructor

spe::UIAssetTools::UIAssetTools()
{
	this->m_IsPopupOpen = false;
	this->m_WindowFontSize = 1;
	this->m_ptr_HoveredIconName = nullptr;
	this->m_ptr_CurrentAssetPath = nullptr;
	this->m_ClassFileName[0] = '\0';
	this->m_OpenFileInput = false;
}

spe::UIAssetTools::UIAssetTools(const std::string* currentAssetPath, std::string* hoveredIconName)
{
	this->m_IsPopupOpen = false;
	this->m_ClassFileName[0] = '\0';
	this->m_OpenFileInput = false;
	this->m_ptr_CurrentAssetPath = currentAssetPath;
	this->m_ptr_HoveredIconName = hoveredIconName;
	this->m_WindowFontSize = 1;
}

// Public functions

void spe::UIAssetTools::Update()
{
	this->GetFileName();

	if (this->m_ClassFileName[0] != '\0' && !this->m_OpenFileInput)
	{
		this->CreateFileContent();
		this->m_ClassFileName[0] = '\0';
	}
	ImGui::SetWindowFontScale(this->m_WindowFontSize);

	if (ImGui::IsMouseReleased(1))
	{
		ImGui::OpenPopup(INPUT_POPUP_NAME);
	}

	if (ImGui::IsPopupOpen(INPUT_POPUP_NAME))
	{
		this->m_IsPopupOpen = true;
		ImGui::BeginPopup(INPUT_POPUP_NAME);
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("C++ Script"))
			{
				//  Create a scirpt, link it into the user project
				this->m_OpenFileInput = true;
			}
			ImGui::EndMenu();
		}
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7);
		if (ImGui::Button("Delete"))
		{
			const std::string d = *this->m_ptr_CurrentAssetPath;
		    std::string m = *this->m_ptr_HoveredIconName;
			
			if (m != "")
			{
				std::string file_name = "";

				for (int i = 2; i < m.size(); i++)
				{
					file_name.push_back(m[i]);
				}

				const std::string delete_path = d + "\\" + file_name;

				spe::Utility::Delete(delete_path);

				*this->m_ptr_HoveredIconName = "";
			}
		}
		ImGui::EndPopup();
	}
	else
	{
		this->m_IsPopupOpen = false;
	}
	ImGui::SetWindowFontScale(spe::Style::s_DefaultFontSize);
}

void spe::UIAssetTools::GetFileName()
{
	if (!this->m_OpenFileInput)
	{
		return;
	}

	if (ImGui::Begin("##input_file_name", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::SetNextItemWidth(290);
		ImGui::InputTextWithHint("##file_input", "<name>", this->m_ClassFileName, CHAR_MAX);
		if (ImGui::Button("Create"))
		{
			this->m_OpenFileInput = false;
		}

		spe::UIUtility::SetWindowScreenMiddle(ImVec2(300, 100));
		ImGui::End();
	}
}

void spe::UIAssetTools::CreateFileContent()
{
	std::string header_content =
		"#pragma once\n\n"
		"#include <_header/SpriteEngine.h>\n\n"
		"class " + std::string(this->m_ClassFileName) + " : public spe::Base\n"
		"{\n"
		"public:\n"
		"   void update();\n"
		"   void start();\n"
		"};\n";

	std::string cpp_content =
		"#include \"" + std::string(this->m_ClassFileName) + ".h\"\n\n"
		"void " + std::string(this->m_ClassFileName) + "::start()\n"
		"{\n"
		"\n"
		"}\n"
		"\n"
		"void " + std::string(this->m_ClassFileName) + "::update()\n"
		"{\n"
		"\n"
		"}\n"
		"\n";

	std::string path = *this->m_ptr_CurrentAssetPath + "\\" + this->m_ClassFileName;

	std::string cpp_name = std::string(this->m_ClassFileName);
	std::string header_name = std::string(this->m_ClassFileName);

	std::string cpp_file_path = *this->m_ptr_CurrentAssetPath + "\\" + header_name + ".cpp";
	std::string hpp_file_path = *this->m_ptr_CurrentAssetPath + "\\" + header_name + ".h";

	spe::Utility::CreateFileWithContent(header_content, hpp_file_path);
	spe::Utility::CreateFileWithContent(cpp_content, cpp_file_path);

	// Building the project of the user when he inserts a new thingy
	spe::EngineData::BuildProject();
}
