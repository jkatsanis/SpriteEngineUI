#pragma once

#include <UIHierarchy.h>
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
		UIHierarchy m_UIHierarchy;
		UIToolButtons m_UIToolButtons;
		UIInspector m_UIInspector;
		UIAssetFolder m_UIAssetFolder;
		UIAnimation m_UIAnimation;

		const SpriteRepository* m_ptr_repo;

	public:
		bool areAnyUIWindowsHovered;

		UIWindow();

	    void update();
		void init(s2d::SpriteRepository& repo);

		UIInspector& getInspector() { return this->m_UIInspector; }

	public:
		static void renderStyle(ImGuiStyle* style);
	};
}


