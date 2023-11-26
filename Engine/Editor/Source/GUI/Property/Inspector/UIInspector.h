#pragma once

#include <iostream>
#include <ImGui.h>
#include <Include/UtilityFunctions.h>
#include <Include/SFML/Graphics.hpp>

#include "RessourceHandler/FileDataMacros.h"
#include "GUI/Property/Inspector/UITagSelector.h"
#include "GUI/Property/Inspector/UIInspectorBoxCollider.h"
#include "GUI/UIUtility/UIModels.h"
#include "GUI/Property/UIBase.h"
#include "Utility/Style.h"
#include "GUI/UIUtility/UIUtility.h"
#include "Core/Time.h"
#include "GUI/Color.h"
#include "Savesystem.h"

#include "Sprite/Sprite.h"

#define DEFAULT_BACKGROUND_COLOR spe::Vector3(139, 165, 187)
#define INSPECTOR_WINDOW_POS ImVec2(1530, 53)
#define INSPECTOR_DEFAULT_WINDOW_SIZE ImVec2(390, 1000)
#define ADD_COMPONENTS_MARGIN 300
#define SEARCH_BAR_MARGIN 290
#define COMPONENT_PADDING_LEFT 20
#define COMPONENT_SELECTED_COLOR SPRITE_BACKGROUND_COLOR
#define DUMMY_COMPONENT ImGui::Dummy(ImVec2(0, 8))
#define DUPE_NAME_TIME_EDIT 0.3f

namespace spe
{
	enum class InspectorState
	{
		None = -1,
		GameWindowEditor = 0,
		SpriteEditorWindow = 1
	};
	class UIInspector : public spe::IUIBase
	{
	private:
		// gui repo (rectangles)
		spe::Rectangle* m_ptr_collider_rectangle;
		spe::Rectangle* m_ptr_sprite_over_rectangle;

		spe::UITagSelector m_tag_selector;
		spe::UIInspectorBoxCollider m_collider;

		spe::ResizeWindowData m_resize_window_data;
		std::string m_menu_name;
		std::string m_sprite_name;
;
		ImVec2 m_pop_up_cursor_pos;
		float m_sprite_input_width;
		float m_dupe_name_editor_timer;
		std::vector<const char*> m_components;

		//When the box collider component is open we want to display the rectangle as a box collider and not as a rectangle
		float m_window_size_width;
		const char* m_current_component_selected;
		char* m_input_name;

		float m_light_radius;
		float m_light_intensity;
		float m_cam_zoom;

		void renderOptions();
		void resizeWindow();
		void setupComponents();

		void transformComponent();
		void spriteRendererComponent();
		void boxColliderComponent();
		void physicsBodyComponent();
		void animatorComponent();
		void prefabComponent();
		void lightComponent();

		void displayDefaultInspectorView();
	
		void renameSprite();
		void componentSelector();
		void setCompontents();
		void drawRectangleOverCurrentObject();
		void renderBackgroundBehindComponent();
		void backgroundSetting();
		void gameEngineViewSetting();
		void renderComponentOptions(spe::Component& component, const std::string& name);
		void generalSettings();

		void Init() override;

	public:
		bool is_hovered;
		spe::InspectorState state;

		void Render() override;
	};
}

