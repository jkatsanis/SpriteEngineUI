#include "fontManager.h"
#include <iostream>
void s2d::FontManager::displaySmybolAsText(const char* symbol)
{
	ImGui::PopFont();
	ImGui::PushFont(s2d::FontManager::symbolFont);

	ImGui::Text(symbol);

	ImGui::PopFont();
	ImGui::PushFont(s2d::FontManager::defaultFont);
}

bool s2d::FontManager::displaySmybolAsButton(const char* symbol, ImFont* font)
{
	ImGui::PopFont();

	bool clicked = false;

	if (font != nullptr)
	{
		ImGui::PushFont(font);

		clicked = ImGui::Button(symbol, ImVec2(50, 50));
	}
	else
	{
		ImGui::PushFont(s2d::FontManager::symbolFont);

	    clicked = ImGui::Button(symbol);
	}


	ImGui::PopFont();
	ImGui::PushFont(s2d::FontManager::defaultFont);

	return clicked;
}

bool s2d::FontManager::displaySmybolAsButton(const char* symbol, float defaultFontSize)
{
	ImGui::SetWindowFontScale(defaultFontSize);

	bool clicked = false;
	ImGui::PopFont();
	ImGui::PushFont(s2d::FontManager::symbolFont);

	clicked = ImGui::Button(symbol);

	ImGui::PopFont();
	ImGui::PushFont(s2d::FontManager::defaultFont);

	ImGui::SetWindowFontScale(s2d::UIInfo::sdefaultFontSize);
	return clicked;
}


ImFont* s2d::FontManager::defaultFont = nullptr;
ImFont* s2d::FontManager::imGuiDefaultFont = nullptr;
ImFont* s2d::FontManager::symbolFont = nullptr;
float s2d::FontManager::fontSize = 24.0f;
