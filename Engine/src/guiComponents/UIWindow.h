#pragma once

#include <UIHirachy.h>
#include <UIToolButtons.h>
#include <UIInspector.h>
#include <UIAssetFolder.h>
#include <UIAnimation.h>
#include <vector>
#include <manager/spriteRepository.h>

namespace s2d
{
	class UIWindow
	{
	private:
		UIHirachy m_UIHirachy;
		UIToolButtons m_UIToolButtons;
		UIInspector m_UIInspector;
		UIAssetFolder m_UIAssetFolder;
		UIAnimation m_UIAnimation;

	public:
		bool areAnyUIWindowsHovered;

		UIWindow();

	    void update();
		void init(s2d::SpriteRepository& repo);

		UIInspector& getInspector() { return this->m_UIInspector; }
		UIToolButtons& getTools() { return this->m_UIToolButtons; }

	public:
		static void renderStyle(ImGuiStyle* style);
	};
}


