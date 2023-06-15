#pragma once

#include <iostream>
#include <ImGui.h>
#include <stdExtension.h>
#include <SFML/Graphics.hpp>
#include <calculationComponents/vector3.h>
#include <UIInspectorBoxCollider.h>
#include <data/UIInfo.h>
#include <UIAssetFolder.h>
#include <manager/fontManager.h>
#include <manager/spriteRepository.h>
#include <UIHierarchy.h>
#include <spriteComponents/component.h>

#define DEFAULT_BACKGROUND_COLOR s2d::Vector3(139, 165, 187)
#define INSPECTOR_WINDOW_POS ImVec2(1530, 53)
#define INSPECTOR_DEFAULT_WINDOW_SIZE ImVec2(390, 1000)
#define ADD_COMPONENTS_MARGIN 300
#define SEARCH_BAR_MARGIN 290
#define COMPONENT_PADDING_LEFT 20
#define COMPONENT_SELECTED_COLOR SPRITE_BACKGROUND_COLOR
#define DUMMY_COMPONENT ImGui::Dummy(ImVec2(0, 8))
#define DUPE_NAME_TIME_EDIT 0.3f

namespace s2d
{
	enum class InspectorState
	{
		None = -1,
		GameWindowEditor = 0,
		SpriteEditorWindow = 1
	};
	class UIInspector
	{
	private:
		// gui repo (rectangles)
		s2d::GUIRepository* m_ptr_gui_repo;
		s2d::Rectangle* m_ptr_collider_rectangle;
		s2d::Rectangle* m_ptr_sprite_over_rectangle;

		s2d::ResizeWindowData m_resize_window_data;
		s2d::SpriteRepository* m_ptr_sprite_repo;
		std::string m_menu_name;
		std::string m_sprite_name;
;
		ImGuiTextFilter m_search_component_filter;
		ImVec2 m_window_size;
		ImVec2 m_pop_up_cursor_pos;
		float m_sprite_input_width;
		float m_dupe_name_editor_timer;
		std::vector<const char*> m_components;

		//When the box collider component is open we want to display the rectangle as a box collider and not as a rectangle
		float m_window_size_width;
		const char* m_current_component_selected;
		char* m_input_name;
		s2d::UIInspectorBoxCollider m_collider;

		void render();
		void renderOptions();
		void resizeWindow();
		void setupComponents();
		void transformComponent();
		void spriteRendererComponent();
		void boxColliderComponent();
		void physicsBodyComponent();
		void animatorComponent();
		void prefabComponent();
		void componentSelector();
		void setCompontents();
		void drawRectangleOverCurrentObject();
		void renderBackgroundBehindComponent();
		void displayDefaultInspectorView();
		void backgroundSetting();
		void gameEngineViewSetting();
		void preInit();
		void afterInit();
		void renderComponentOptions(s2d::Component& component, const std::string& name);
		void editDupeName();

	public:
		bool is_hovered;
		s2d::InspectorState state;

		UIInspector();

		void createUIInspector();

		void setSpriteRepository(s2d::SpriteRepository& repo) { this->m_ptr_sprite_repo = &repo; }
		void setGUIRepo(s2d::GUIRepository* repo, s2d::Event* evnt);

	public:
		static std::string getNamePathSplit(std::string path);
	};
}

