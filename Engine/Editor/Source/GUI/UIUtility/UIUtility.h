#pragma once

#include <ImGui/ImGUISFML.h>
#include <iostream>

#include "Components/Transform.h"
#include "Math/Vector2.h"
#include "GUI/UIUtility/UIModels.h"
#include "Utility/Style.h"
#include "Core/Event.h"

#define INVALID_SCALE 1.11f
#define CURSOR_HITBOX 20

namespace spe
{
	class UIUtility
	{
	private:
		static spe::Event* s_m_ptr_Event;
		static sf::RenderWindow* s_m_ptr_Window;

		static float scaleChanger(spe::ScaleDott& dott, float default_size, float pos_o, bool x);
	public:
		static spe::Vector2 GUICursor;
		static sf::Vector2f WorldCursor;

		UIUtility() = delete;

		static void UpdateCursor();

		static float xScaleChanger(spe::ScaleDott& dott, float default_size, float pos_x);
		static float yScaleChanger(spe::ScaleDott& dott, float default_size, float pos_y);

		static bool isCursorClickedOnSprite(const spe::Sprite* check);
		static spe::Vector2 getWorldCordinates();
		static void setCursorToWorldCoordinates(const spe::Vector2& vec);
		static bool isCursorClickedOnRectangle(const sf::RectangleShape& shape);

		static void SetWindowScreenMiddle(const ImVec2& ref);
		static void SameLine(float width);
		static bool RenderCloseRectangle(float padding_left, const char* icon, const std::string& id,
			const std::string& content, float cursor_pos);
		static void DrawRectangleInGUIWIndow(const ImVec2& size, const ImVec2& top_left, const ImColor& color);
		static bool IsHovered(const ImVec2& windowPos, const ImVec2& windowSize);
		static bool HandleCloseAndReloadWindow(spe::UIWindowData& data, bool& hovered, const ImVec2& original_size);

		static void SetEvent(spe::Event* evnt) { spe::UIUtility::s_m_ptr_Event = evnt; }
		static void SetRenderWinodw(sf::RenderWindow* window) { spe::UIUtility::s_m_ptr_Window = window; }

		static bool s_IsAnyHovered;
	};
}