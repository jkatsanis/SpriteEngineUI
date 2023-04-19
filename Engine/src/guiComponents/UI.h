#pragma once

#include <physicalComponents/sprite.h>
#include <engineComponents/event.h>
#include <spriteComponents/stransform.h>

namespace s2d
{
	class UI
	{
	private:
		static const sf::RenderWindow* s_ptr_Window;
		static sf::Vector2f s_cursorWorldPos;
		static short s_cursorHitbox;
		static s2d::Event* s_event;

	public:
		UI() = delete;

		static s2d::Transform s_gameCursor;
		static s2d::Transform s_guiCorsor;

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

		static void update();
	};
}

