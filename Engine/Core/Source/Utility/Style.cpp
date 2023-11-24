#include "Style.h"

bool spe::Style::DisplaySymbolAsButtonWithWidthAndCursorPos(const char* symbol, const ImVec2& cursor, const ImVec2& width, const std::string& identy)
{
	ImGui::SetCursorPos(cursor);
	bool clicked = false;
	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_SymbolFont);

	const std::string identy_button = identy + "-button";

	clicked = ImGui::Button(identy_button.c_str(), width);

	ImGui::SetCursorPos(ImVec2(cursor.x + 7, ImGui::GetCursorPosY() - 25));
	ImGui::Text(symbol);

	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_DefaultFont);
	return clicked;
}

bool spe::Style::DisplaySymbolInMenuItem(const char* symbol)
{
	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_SymbolFont);
	bool clicked = ImGui::MenuItem(symbol);

	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_DefaultFont);

	return clicked;
}

void spe::Style::DisplaySmybolAsText(const char* symbol)
{
	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_SymbolFont);

	ImGui::Text(symbol);

	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_DefaultFont);
}

bool spe::Style::DisplaySmybolAsButton(const char* symbol, ImFont* font)
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
		ImGui::PushFont(spe::Style::s_SymbolFont);

		clicked = ImGui::Button(symbol);
	}


	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_DefaultFont);

	return clicked;
}

bool spe::Style::DisplaySmybolAsButton(const char* symbol, float defaultFontSize)
{
	ImGui::SetWindowFontScale(defaultFontSize);

	bool clicked = false;
	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_SymbolFont);

	clicked = ImGui::Button(symbol);

	ImGui::PopFont();
	ImGui::PushFont(spe::Style::s_DefaultFont);

	return clicked;
}

bool spe::Style::DisplaySmybolAsButton(const char* symbol, ImVec2 cursorPos)
{
	ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y));
	return DisplaySmybolAsButton(symbol);
}

bool spe::Style::DisplaySymbolInTreeNode(const char* symbol, std::string name, bool openNode)
{
	const ImVec2 cursor = ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPosY());
	ImGui::SetCursorPos(ImVec2(cursor.x + 35, cursor.y));
	spe::Style::DisplaySmybolAsText(symbol);
	ImGui::SetCursorPos(cursor);

	const std::string whiteSpaces = std::string("    ") + name;
	name = whiteSpaces.c_str();
	if (openNode)
		ImGui::SetNextItemOpen(true);
	return ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
}

bool spe::Style::DisplaySymbolInMenuItemWithText(const char* symbol, std::string name)
{
	const ImVec2 cursor = ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPosY());
	ImGui::SetCursorPos(ImVec2(cursor.x + 30, cursor.y));
	spe::Style::DisplaySmybolAsText(symbol);
	ImGui::SetCursorPos(cursor);

	const std::string whiteSpaces = std::string("           ") + name;
	name = whiteSpaces.c_str();
	return ImGui::MenuItem(name.c_str());
}


void spe::Style::Init()
{
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig CustomFont;
	ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	io.IniFilename = nullptr;
	CustomFont.FontDataOwnedByAtlas = false;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	ImFontConfig config;

	static const ImWchar ranges[] =
	{
		0xf0c7, 0xf0c7, // Save
		0xf06e, 0xf06e, // Eye
		0xf070, 0xf070, // Eye slashed
		0xf013, 0xf013, // Zahnrad
		0xf1b2, 0xf1b2, // Cube
		0xf060, 0xf060, // Arrow left
		0xf061, 0xf061, // Arrow right
		0xf062, 0xf062, // Arrow up
		0xf1c9, 0xf1c9, // File Codes
		0xf37e, 0xf37e, // Browser
		0xf07b, 0xf07b, // Folder
		0xf0c8, 0xf0c8, // Square
		0xf04b, 0xf04b, // Play
		0xf047, 0xf047, // Arrows
		0xf002, 0xf002, // Search
		0xf067, 0xf067, // Plus
		0xf044, 0xf044, // Edit
		0xf1f8, 0xf1f8, // Trash
		0xf079, 0xf079, // Retweet
		0xf04b, 0xf04b, // Play button
		0xf03e, 0xf03e, // Image
		0xf15b, 0xf15b, // File
		0,
	};

	config.GlyphRanges = ranges;

	//Add the fonts (remember to fill in the correct path of your font
	spe::Style::s_DefaultFont = io.Fonts->AddFontFromFileTTF(PATH_TO_RESSOURCES"\\Fonts\\arial.ttf", spe::Style::s_FontSize);
	spe::Style::s_SymbolFont = io.Fonts->AddFontFromFileTTF(PATH_TO_RESSOURCES"\\Fonts\\fontawesome-webfont.ttf", spe::Style::s_FontSize - 4, &config);

	//This function is important else the program will crash with an assertion
	ImGui::SFML::UpdateFontTexture();
}

void spe::Style::RenderStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	ImVec4 shadow_color = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);

	//Setting it centered
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->FramePadding = ImVec2(8, 6);

	style->Colors[ImGuiCol_TitleBg] = ImColor(49, 49, 76);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(49, 49, 76);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(217, 101, 53, 255);

	//the sprite selected
	style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(100, 90, 100, 100);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);

	style->Colors[ImGuiCol_Button] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(48, 48, 48, 0);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(100, 90, 100, 100);

	//The background of (input) 
	style->Colors[ImGuiCol_FrameBg] = shadow_color;
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(45, 45, 45);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50);

	style->Colors[ImGuiCol_WindowBg] = ImColor(36, 36, 36);

	style->Colors[ImGuiCol_TextSelectedBg] = ImColor(30, 30, 30);

	style->Colors[ImGuiCol_SliderGrab] = ImColor(30, 30, 30);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(25, 25, 25);

	style->Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);

	style->Colors[ImGuiCol_ChildBg] = ImColor(26, 26, 26);
	style->Colors[ImGuiCol_MenuBarBg] = ImColor(26, 26, 26, 1);

	style->FrameRounding = 4.0f;
	style->GrabRounding = 4.0f;
}


ImFont* spe::Style::s_DefaultFont = nullptr;
ImFont* spe::Style::s_SymbolFont = nullptr;
float spe::Style::s_FontSize = 20.0f;
float spe::Style::s_DefaultFontSize = 0.9f;

