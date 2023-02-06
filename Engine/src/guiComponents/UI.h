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

		static s2d::Transform cursor;

		static bool isCursorClickedOnSprite(const s2d::Sprite* check);
		static s2d::Vector2 getWorldCordinates();
		static void setCursorToWorldCoordinates(const s2d::Vector2& vec);
		static bool isCursorClickedOnRectangle(const sf::RectangleShape& shape);

		static void setRenderWindow(const sf::RenderWindow* window);
		static void setS2DEvent(s2d::Event* const event);
	};
}

