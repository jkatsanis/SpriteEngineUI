#pragma once

#include <sprite.h>
#include <ImGui.h>
#include <icons.h>
#include <UIHirachy.h>
#include <UIAssetFolder.h>

namespace s2d
{
	class UIInspectorBoxCollider
	{
	private:	
		sf::Texture texture;

		void firstReset(float& x, float& y, s2d::Sprite* sprite);
		void secondReset(float& x, float& y, s2d::Sprite* sprite);

		void x(s2d::Sprite* sprite);
		void y(s2d::Sprite* sprite);

	public:
		UIInspectorBoxCollider();

		void edit(float& x, float& y);
	    void solid(float& x, float& y, s2d::Sprite* sprite);
		void width(float x, float y, s2d::Sprite* sprite);
		void height(s2d::Sprite* sprite);
		void drawBoxCollider(s2d::Sprite* sprite);
	};
}