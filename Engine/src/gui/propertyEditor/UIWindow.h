#pragma once

#include <UIHirachy.h>
#include <UIToolButtons.h>
#include <UIInspector.h>
#include <UIAssetFolder.h>
#include <UIAnimation.h>
#include <vector>

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

		void renderStyle();
		Vector3 getWindowBackgroundColorFromFile();

	public:
		bool isAnyUIWindowHovered;

		UIWindow();

	    void update();

		UIInspector& getInspector() { return this->m_UIInspector; }
	};
}


