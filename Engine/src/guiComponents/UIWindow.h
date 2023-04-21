#pragma once

#include <UIHierarchy.h>
#include <UIToolButtons.h>
#include <UIInspector.h>
#include <UIAssetFolder.h>
#include <UIAnimation.h>
#include <vector>
#include <manager/spriteRepository.h>
#include <manager/guiRepository.h>

namespace s2d
{
	class UIWindow
	{
	private:
		UIHierarchy m_ui_hierarchy;
		UIToolButtons m_ui_tool_button;
		UIInspector m_ui_inspector;
		UIAssetFolder m_ui_asset_folder;
		UIAnimation m_ui_animation;

		GUIRepository m_gui_repository;
		const SpriteRepository* m_ptr_repo;

	public:
		bool ary_any_windows_hovered;

		UIWindow();

	    void update();
		void init(s2d::SpriteRepository& repo);

		UIInspector& getInspector() { return this->m_ui_inspector; }

	public:
		static void renderStyle(ImGuiStyle* style);
	};
}


