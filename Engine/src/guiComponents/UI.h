#pragma once

#include <physicalComponents/sprite.h>
#include <engineComponents/event.h>
#include <manager/fontManager.h>
#include <spriteComponents/stransform.h>
#include <guiComponents/UIModels.h>

#define INVALID_SCALE 1.11f

namespace s2d
{
	class UI
	{
	private:
		static const sf::RenderWindow* s_ptr_Window;
		static sf::Vector2f s_cursor_world_pos;
		static short s_cursor_hit_box;
		static s2d::Event* s_event;

		static float scaleChanger(s2d::ScaleDott& dott, float default_size, float pos_x, bool x);
	public:
		UI() = delete;

		static s2d::Transform s_game_cursor;
		static s2d::Transform s_gui_cursor;

		static bool isCursorClickedOnSprite(const s2d::Sprite* check);
		static s2d::Vector2 getWorldCordinates();
		static void setCursorToWorldCoordinates(const s2d::Vector2& vec);
		static bool isCursorClickedOnRectangle(const sf::RectangleShape& shape);

		static void setRenderWindow(const sf::RenderWindow* window);
		static void setS2DEvent(s2d::Event* const event);

		/// <summary>
		/// For example "..\\..\\LOL\\UserProject\\assets\\"
		/// </summary>
		/// <returns>Path from the user project to the file/folder</returns>
		static std::string getUserProjectPathSeperatetFromEnginePath(const std::string& path);

		/// <summary>
		/// Only use this method when you have a active window
		/// </summary>
		static void setWindowScreenMiddle(const s2d::Vector2& size);

		/// <summary>
		/// Checks if the given area is hovered, does ignore literally everything
		/// </summary>
		/// <returns>True if hovered</returns>
		static bool isHovered(const ImVec2& windowPos, const ImVec2& windowSize);

		static bool renderCloseRectangle(float padding_left, const char* icon, const std::string& id, 
			const std::string& content, float cursor_pos);

		static bool handleCloseAndReloadWindow(bool open, bool& reload, bool& hovered, ImVec2& window_size, const ImVec2& original_size);

		static void drawRectangleInGUIWIndow(const ImVec2& size, const ImVec2& top_left, const ImColor& color);

		static float xScaleChanger(s2d::ScaleDott& dott, float default_size, float pos_x);
		static float yScaleChanger(s2d::ScaleDott& dott, float default_size, float pos_y);

		static void update();
	};
}

